#pragma once 

#ifndef _GSM_H
#define _GSM_H

#include <MKRGSM.h>
#include "bikestate.h"

class GSMInterface
{

	public:
		GSMInterface(long timeout);
        ~GSMInterface();
	    void doNetworkStuff(bikedata *data);
        void setup();
        // originally in while loop with break
	    // This function use the location's APIs to get the device coordinates and update the global variable if all the requirement are satisfied
		//send json data to endpoint

	private:
        enum State
        {
            DISCONNECTED,
            READY,
            SENDING,
            RECEIVING,
        };
        State _currentState;
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