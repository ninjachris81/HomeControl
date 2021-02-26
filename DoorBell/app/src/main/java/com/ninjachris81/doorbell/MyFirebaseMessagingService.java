/*
package com.ninjachris81.doorbell;

import android.content.Intent;
import android.util.Log;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.time.Duration;
import java.util.Date;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

public class MyFirebaseMessagingService extends FirebaseMessagingService {

    public static final String TAG = "MyFirebaseMessagingService";

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        Log.i(TAG, "Message from " + remoteMessage.getFrom());

        String tsStr = remoteMessage.getNotification().getBody();
        Long ts = Long.parseLong(tsStr);
        Date date = new Date(ts * 1000);
        Duration diff = Duration.between(date.toInstant(), new Date().toInstant());
        if (Math.abs(diff.getSeconds()) < 30) {
            Log.i(TAG, "Raising ring event");
            LocalBroadcastManager.getInstance(this).sendBroadcast(new Intent(BackgroundService.BROADCAST_RING_EVENT_RAISE));
        } else {
            Log.i(TAG, "Message too old");
            Intent intent = new Intent(BackgroundService.BROADCAST_RING_EVENT_ADD);
            intent.putExtra("ts", ts);
            LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
        }
    }
}
 */