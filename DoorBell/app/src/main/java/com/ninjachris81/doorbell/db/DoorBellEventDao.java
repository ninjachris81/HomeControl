package com.ninjachris81.doorbell.db;

import java.util.ArrayList;
import java.util.List;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.Query;

@Dao
public interface DoorBellEventDao {
    @Query("SELECT COUNT(*) FROM DoorBellEvent WHERE ts=:ts")
    int count(long ts);

    @Query("SELECT MAX(ts) FROM DoorBellEvent")
    long getMax();

    @Query("SELECT * FROM DoorBellEvent ORDER BY ts DESC")
    List<DoorBellEvent> getAll();

    @Insert
    long insertEvent(DoorBellEvent event);

    @Insert
    void insertAll(DoorBellEvent... events);

    @Delete
    void delete(DoorBellEvent events);
}