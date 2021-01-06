package com.ninjachris81.doorbell;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.media.MediaPlayer;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.CountDownTimer;
import android.os.IBinder;
import android.util.Log;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.ninjachris81.doorbell.db.DoorBellEvent;
import com.ninjachris81.doorbell.db.EventDatabase;
import com.ninjachris81.doorbell.mqtt.ActionListener;
import com.ninjachris81.doorbell.mqtt.Connection;
import com.ninjachris81.doorbell.mqtt.Connections;
import com.ninjachris81.doorbell.mqtt.IReceivedMessageListener;
import com.ninjachris81.doorbell.mqtt.MqttCallbackHandler;
import com.ninjachris81.doorbell.mqtt.ReceivedMessage;
import com.ninjachris81.doorbell.mqtt.Subscription;

import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicBoolean;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;
import androidx.preference.PreferenceManager;
import androidx.room.Room;

import static com.ninjachris81.doorbell.MainActivity.INTENT_ACTION_CANCEL;

public class BackgroundService extends Service {

    public static final String BROADCAST_DATA = "com.ninjachris81.doorbell.data";

    public static final String BROADCAST_RING_EVENT_RAISE = "com.ninjachris81.doorbell.ringEvent.raise";
    public static final String BROADCAST_RING_EVENT_ADD = "com.ninjachris81.doorbell.ringEvent.add";
    public static final String BROADCAST_RING_EVENT_BEGIN = "com.ninjachris81.doorbell.ringEvent.begin";
    public static final String BROADCAST_RING_EVENT_END = "com.ninjachris81.doorbell.ringEvent.end";
    public static final String BROADCAST_RING_EVENT_CANCEL = "com.ninjachris81.doorbell.ringEvent.cancel";

    private static final String DOOR_BELL_CHANNEL_ID = "com.ninjachris81.doorbell.BELL_CHANNEL";

    String clientId = "DoorBellClient";

    final String subscriptionTopic = "hc/events/val/0";

    public static final String INTENT_FILTER = "new_data";
    public static final IntentFilter NEW_DATA = new IntentFilter(INTENT_FILTER);

    private static final String TAG = "BackgroundService";

    RequestQueue requestQueue;

    private SharedPreferences prefs;

    Executor myDBExecutor = Executors.newSingleThreadExecutor();

    // handler for received data from service
    private final BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(BROADCAST_RING_EVENT_CANCEL)) {
                Log.i(TAG, "Cancel ring");
                if (currentRingTimer!=null) {
                    Log.i(TAG, "onFinish");
                    currentRingTimer.onFinish();
                    currentRingTimer.cancel();
                }
            } else if (intent.getAction().equals(BROADCAST_RING_EVENT_RAISE)) {
                newRingEvent();
            } else if (intent.getAction().equals(BROADCAST_RING_EVENT_ADD)) {
                checkInsertTs(intent.getLongExtra("ts", 0));
                updateDataToUi();
            }
        }
    };

    Connection connection;
    Subscription subscription;

    int lastEventId = -1;

    public BackgroundService() {
        Log.d(TAG, "New Background Service");
    }

    private Ringtone currentRingtone = null;
    private CountDownTimer currentRingTimer = null;

    private EventDatabase db;

    private AtomicBoolean isRinging = new AtomicBoolean(false);

    MediaPlayer knockMediaPlayer;
    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        Log.d(TAG, "New Background Service started");

        prefs = PreferenceManager.getDefaultSharedPreferences(this);
        //prefs.edit().clear().commit();

        isRinging.set(false);

        db = Room.databaseBuilder(getApplicationContext(), EventDatabase.class, "door-bell-events").build();

        syncData();

        IntentFilter filter = new IntentFilter();
        filter.addAction(BackgroundService.BROADCAST_RING_EVENT_CANCEL);
        filter.addAction(BackgroundService.BROADCAST_RING_EVENT_RAISE);
        filter.addAction(BackgroundService.BROADCAST_RING_EVENT_ADD);
        LocalBroadcastManager.getInstance(this).registerReceiver(mBroadcastReceiver, filter);

        knockMediaPlayer = MediaPlayer.create(this, R.raw.knock);

        final String clientHandle = "clientId" + System.currentTimeMillis();
        connection = Connection.createConnection(clientHandle, clientId, prefs.getString("settings_mqtt_host", "rpi-server.fritz.box"), 1883, this, false);
        connection.addConnectionOptions(new MqttConnectOptions());
        connection.getConnectionOptions().setCleanSession(true);
        Connections.getInstance(this).addConnection(connection);

        connection.addReceivedMessageListener(new IReceivedMessageListener() {
            @Override
            public void onMessageReceived(ReceivedMessage message) {
                Log.i(TAG, "Message " + message.toString());

                String payload = new String(message.getMessage().getPayload());
                if (payload.startsWith("i")) {
                    int eventId = Integer.parseInt(payload.substring(1));
                    if (lastEventId != eventId) {
                        lastEventId = eventId;
                        newRingEvent();
                    }
                } else {
                    Log.w(TAG, "Unknown value type - expected integer");
                }
            }
        });

        String[] actionArgs = new String[1];
        actionArgs[0] = connection.getId();
        final ActionListener callback = new ActionListener(this,
                ActionListener.Action.CONNECT, connection, actionArgs) {
            @Override
            public void onSuccess(IMqttToken asyncActionToken) {
                Log.i(TAG, "Connected");

                subscription = new Subscription(subscriptionTopic, 1, clientHandle, false);
                try {
                    connection.addNewSubscription(subscription);
                } catch (MqttException e) {
                    Log.e(this.getClass().getCanonicalName(),
                            "Subscribe failed", e);
                }

                super.onSuccess(asyncActionToken);
            }
        };
        connection.getClient().setCallback(new MqttCallbackHandler(this, connection.handle()));
        try {
            connection.getClient().connect(connection.getConnectionOptions(), null, callback);
        }
        catch (MqttException e) {
            Log.e(this.getClass().getCanonicalName(),
                    "MqttException occurred", e);
        }

        createNotificationChannel();

        return START_REDELIVER_INTENT;
    }

    private void newRingEvent() {
        Log.i(TAG, "New Ring event");

        final LocalBroadcastManager bm = LocalBroadcastManager.getInstance(this);

        if (!isRinging.get()) {
            isRinging.set(true);

            if (!prefs.getBoolean("settings_silent_mode", false)) {
                Uri notification = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
                currentRingtone = RingtoneManager.getRingtone(getApplicationContext(), notification);
                currentRingtone.play();
            }

            currentRingTimer = new CountDownTimer(10000, 5000) {

                @Override
                public void onTick(long l) {
                    if (!prefs.getBoolean("settings_silent_mode", false)) {
                        knockMediaPlayer.start(); // no need to call prepare(); create() does that for you
                    }
                }

                @Override
                public void onFinish() {
                    bm.sendBroadcast(new Intent(BROADCAST_RING_EVENT_END));

                    if (currentRingtone!=null) currentRingtone.stop();
                    currentRingtone = null;
                    isRinging.set(false);
                }
            }.start();

            Intent intent = new Intent(this, NotificationActivity.class);
            intent.setAction(INTENT_ACTION_CANCEL);
            PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, 0);

            NotificationCompat.Builder builder = new NotificationCompat.Builder(this, DOOR_BELL_CHANNEL_ID)
                    .setSmallIcon(R.drawable.ic_bell)
                    .setContentTitle(getResources().getString(R.string.ring_title))
                    .setContentText(getResources().getString(R.string.ring_title))
                    .setContentIntent(pendingIntent)
                    .setAutoCancel(true)
                    .setPriority(NotificationCompat.PRIORITY_DEFAULT);

            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
            int notificationId = 1111;
            notificationManager.notify(notificationId, builder.build());

            checkInsertTs(System.currentTimeMillis() / 1000);
            updateDataToUi();

            bm.sendBroadcast(new Intent(BROADCAST_RING_EVENT_BEGIN));
        }
    }

    private void checkInsertTs(long ts) {
        myDBExecutor.execute(() -> {
            if (db.eventDao().count(ts)==0) {
                db.eventDao().insertEvent(new DoorBellEvent(ts));
            } else {
                Log.w(TAG, "Element " + ts + " already exists");
            }
        });
    }

    private void updateDataToUi() {
        myDBExecutor.execute(() -> {
            final LocalBroadcastManager bm = LocalBroadcastManager.getInstance(this);

            Intent intent = new Intent(BROADCAST_DATA);
            intent.putParcelableArrayListExtra("tsList", new ArrayList<>(db.eventDao().getAll()));

            bm.sendBroadcast(intent);
        });
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            Log.i(TAG, "Creating notification channel");
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(DOOR_BELL_CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    @Override
    public IBinder onBind(Intent arg0) {
        // TODO Auto-generated method stub
        return null;
    }


    void syncData() {
        Log.d(TAG, "syncData");

        myDBExecutor.execute(() -> {
            long lastTs = db.eventDao().getMax();

            Log.i(TAG, "LastTs: " + lastTs);

            requestQueue = Volley.newRequestQueue(this);

            try {
                URL requestUrl = new URL("http", prefs.getString("settings_rest_host", "rpi-server.fritz.box"), Integer.parseInt(prefs.getString("settings_rest_port", "9090")), "/events/last?lastTs=" + lastTs + "&limit=" + Integer.parseInt(prefs.getString("settings_max_entries", "1000")));

                Log.i(TAG, "Requesting " + requestUrl.toString());

                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest
                        (Request.Method.GET, requestUrl.toString(), null, new Response.Listener<JSONObject>() {

                            @Override
                            public void onResponse(final JSONObject response) {
                                Log.d(TAG, "response: " + response.toString());

                                JSONArray arr = response.optJSONArray("tsList");
                                for (int i=0;i<arr.length();i++) {
                                    checkInsertTs(arr.optLong(i));
                                }
                                updateDataToUi();
                            }
                        }, new Response.ErrorListener() {

                            @Override
                            public void onErrorResponse(VolleyError error) {
                                // TODO: Handle error
                                Log.e(TAG, "Failed to request " + error.toString());
                            }
                        });

                requestQueue.add(jsonObjectRequest);
            } catch (MalformedURLException e) {
                e.printStackTrace();
            }
        });
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        Intent restartServiceIntent = new Intent(this ,this.getClass());
        restartServiceIntent.setPackage(getPackageName());
        startService(restartServiceIntent);
        super.onTaskRemoved(rootIntent);
    }

}
