package com.ninjachris81.doorbell;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

public class NotificationActivity extends AppCompatActivity {

    public static final String TAG = "NotificationActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (getIntent().getAction().equals(MainActivity.INTENT_ACTION_CANCEL)) {
            Log.i(TAG, "Intent Cancel received");
            LocalBroadcastManager.getInstance(this).sendBroadcast(new Intent(BackgroundService.BROADCAST_RING_EVENT_CANCEL));
        }

        this.finish();
    }

}