package com.ninjachris81.doorbell;

import android.content.Context;
import android.text.format.DateUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.ninjachris81.doorbell.db.DoorBellEvent;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import androidx.annotation.NonNull;

public class DoorBellEventAdapter extends ArrayAdapter<DoorBellEvent> {
    SimpleDateFormat sdf1 = new SimpleDateFormat("hh:mm");
    SimpleDateFormat sdf2 = new SimpleDateFormat("dd. MMM");

    public DoorBellEventAdapter(@NonNull Context context, int resource, @NonNull List<DoorBellEvent> objects) {
        super(context, resource, objects);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        DoorBellEvent event = getItem(position);

        if (convertView == null) {
            convertView = LayoutInflater.from(getContext()).inflate(R.layout.event_list_item, parent, false);
        }

        TextView label = convertView.findViewById(R.id.label);

        Date date = new Date(event.ts * 1000);
        label.setText(sdf1.format(date) + " " + getContext().getResources().getString(R.string.hour_format) + " (" + (DateUtils.isToday(date.getTime()) ? getContext().getResources().getString(R.string.today) : sdf2.format(date)) + ")");

        return convertView;
    }

}
