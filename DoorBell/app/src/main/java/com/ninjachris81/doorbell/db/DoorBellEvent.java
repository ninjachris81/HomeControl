package com.ninjachris81.doorbell.db;

import android.os.Parcel;
import android.os.Parcelable;

import androidx.room.Entity;
import androidx.room.PrimaryKey;

@Entity
public class DoorBellEvent implements Parcelable {
    @PrimaryKey
    public long ts;

    public DoorBellEvent() {
    }

    public DoorBellEvent(long ts) {
        this.ts = ts;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeLong(ts);
    }

    public static final Parcelable.Creator<DoorBellEvent> CREATOR
            = new Parcelable.Creator<DoorBellEvent>() {
        public DoorBellEvent createFromParcel(Parcel in) {
            return new DoorBellEvent(in);
        }

        public DoorBellEvent[] newArray(int size) {
            return new DoorBellEvent[size];
        }
    };

    private DoorBellEvent(Parcel in) {
        ts = in.readLong();
    }
}