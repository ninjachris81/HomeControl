package com.ninjachris81.doorbell;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.Fragment;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;

public class BellFragment extends Dialog implements DialogInterface.OnClickListener {

    public static String TAG = "BellFragment";

    public BellFragment(@NonNull Context context) {
        super(context);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        LayoutInflater inflater = getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_bell, null);
        setContentView(view);

        ScaleAnimation fade_in =  new ScaleAnimation(0.5f, 1f, 0.5f, 1f, Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f);
        fade_in.setDuration(800);     // animation duration in milliseconds
        fade_in.setRepeatCount(Animation.INFINITE);
        fade_in.setRepeatMode(Animation.RESTART);
        //fade_in.setFillAfter(true);    // If fillAfter is true, the transformation that this animation performed will persist when it is finished.
        ImageView imageView = findViewById(R.id.ringSymbol);
        imageView.startAnimation(fade_in);

        Button okButton = findViewById(R.id.button);



        okButton.setOnClickListener(l -> {
            LocalBroadcastManager.getInstance(getContext()).sendBroadcast(new Intent(BackgroundService.BROADCAST_RING_EVENT_CANCEL));
            this.dismiss();
        });
    }


    @Override
    public void onClick(DialogInterface dialogInterface, int i) {
        this.dismiss();
    }
}