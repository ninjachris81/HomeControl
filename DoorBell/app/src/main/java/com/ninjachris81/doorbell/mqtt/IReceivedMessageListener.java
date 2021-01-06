package com.ninjachris81.doorbell.mqtt;

public interface IReceivedMessageListener {

    void onMessageReceived(ReceivedMessage message);
}