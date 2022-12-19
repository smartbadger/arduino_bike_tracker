#pragma once

#ifndef _GSM_H
#define _GSM_H
#include <MKRGSM.h>
#include "secrets.h"
#include "debugger.h"
#include "config.h"
#include "StateMachine/StateMachine.h"
#include "models.h"

class GSMInterface
{

public:
    GSMInterface();
    ~GSMInterface();
    void doNetworkStuff(State &state);
    void setup();
    // originally in while loop with break
    // This function use the location's APIs to get the device coordinates and update the global variable if all the requirement are satisfied
    // send json data to endpoint

private:
    enum GSMState
    {
        DISCONNECTED,
        CONNECTING,
        READY,
        IDLE,
        SLEEP,
        SENDING,
        RECEIVING,
    };
    GSMState _currentState;
    GPRS gprs;
    GSM gsmAccess;
    GSM_SMS sms;
    GSMScanner scanner;
    GSMLocation gsmlocation;
    boolean _connected;
    GSMSSLClient client;
    boolean _expired;
    boolean _modemReady;
    unsigned long _timeout;

    Location measureLocation();
    void setExpired(boolean value);
    String getNetworkStatus();
    void connectNetwork();
    void sendDataFromClient();
    void readDataFromServer();
    void sendSMS();
    void readSMS();
};

#endif