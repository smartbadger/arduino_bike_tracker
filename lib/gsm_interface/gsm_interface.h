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
    	void sendData();

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
        boolean _expired;
        boolean _modemReady;
        unsigned long _timeout;

	    Location measureLocation();
        void setExpired(boolean value);
        String getNetworkStatus();
	    //The connectNetwork() function is used for the board data connection
        // originally in while loop with break
	    void connectNetwork();

        // originally in while loop with break
	    void sendText();
};

#endif