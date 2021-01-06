package com.ninjachris81.doorbell.db;

import androidx.room.Database;
import androidx.room.RoomDatabase;

@Database(entities = {DoorBellEvent.class}, version = 1)
public abstract class EventDatabase extends RoomDatabase {
    public abstract DoorBellEventDao eventDao();
}