package com.ninjachris81.doorbell;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;
import androidx.preference.PreferenceManager;
import androidx.work.OneTimeWorkRequest;
import androidx.work.PeriodicWorkRequest;
import androidx.work.WorkRequest;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.ListView;

import com.ninjachris81.doorbell.db.DoorBellEvent;
import com.ninjachris81.doorbell.db.EventDatabase;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    public static final String INTENT_ACTION_CANCEL = "INTENT_CANCEL_BELL";

    private static final String TAG = "MainActivity";

    private BellFragment bellDialog = null;

    //private List<DoorBellEvent> currentData = new ArrayList<>();

    private ArrayAdapter<DoorBellEvent> myArrayAdapter;

    // handler for received data from service
    private final BroadcastReceiver mBroadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(BackgroundService.BROADCAST_RING_EVENT_BEGIN)) {
                onBellBegin();
            } else if (intent.getAction().equals(BackgroundService.BROADCAST_RING_EVENT_END)) {
                onBellEnd();
            } else if (intent.getAction().equals(BackgroundService.BROADCAST_DATA)) {
                Log.i(TAG, "Sync data");
                myArrayAdapter.clear();
                myArrayAdapter.addAll(intent.getParcelableArrayListExtra("tsList"));
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.i(TAG, "onCreate");

        //WorkRequest uploadWorkRequest = new PeriodicWorkRequest.Builder(BackgroundWorker.class, 1, TimeUnit.SECONDS).build();

        IntentFilter filter = new IntentFilter();
        filter.addAction(BackgroundService.BROADCAST_RING_EVENT_BEGIN);
        filter.addAction(BackgroundService.BROADCAST_RING_EVENT_END);
        filter.addAction(BackgroundService.BROADCAST_DATA);
        LocalBroadcastManager.getInstance(this).registerReceiver(mBroadcastReceiver, filter);

        /*
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            startForegroundService(new Intent(getApplicationContext(), BackgroundService.class));
        } else {
        */
            startService(new Intent(getApplicationContext(), BackgroundService.class));
        //}

        myArrayAdapter =
                new DoorBellEventAdapter(
                        this, // Die aktuelle Umgebung (diese Activity)
                        R.layout.event_list_item, // Die ID des Zeilenlayouts (der XML-Layout Datei)
                        new ArrayList<>());

        setContentView(R.layout.activity_main);

        ListView myListView = (ListView) findViewById(R.id.lastEvents);
        myListView.setAdapter(myArrayAdapter);
   }

    protected void onBellBegin() {
        Log.i(TAG, "Bell begin");

        if (bellDialog!=null) {
            bellDialog.dismiss();
        }

        bellDialog = new BellFragment(this);
        bellDialog.show();
    }

    protected void onBellEnd() {
        if (bellDialog!=null) {
            bellDialog.dismiss();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater findMenuItems = getMenuInflater();
        findMenuItems.inflate(R.menu.main_menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.action_settings:
                startActivity(new Intent(this, SettingsActivity.class));
                break;
        }


        return super.onOptionsItemSelected(item);
    }
    }