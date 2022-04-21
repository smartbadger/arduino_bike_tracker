#include <MKRGSM.h>
#include "location.h"

#ifndef _GSM_H
#define _GSM_H
class GSMInterface
{

	public:
		GSMInterface(long timeout);
        ~GSMInterface();
	    void ready();
	    void doNetworkStuff();
        void setup();
        // originally in while loop with break
	    // This function use the location's APIs to get the device coordinates and update the global variable if all the requirement are satisfied
	    Location measureLocation();
		//send json data to endpoint
    	void sendData();

	private:
        enum State
        {
            DISCONNECTED,
            READY,
            SEND_STATUS,
        };
        State _currentState;
        State _targetState;
        GPRS gprs;
        GSM gsmAccess;
        GSM_SMS sms;
        GSMLocation gsmlocation;
        boolean _connected;
        boolean _expired;
        unsigned long _timeout;
    
		void goToDisconnected();
	    void goToReady();
	    void goToSend();
        void connect();

        void setExpired(boolean value);

	    //The connectNetwork() function is used for the board data connection
        // originally in while loop with break
	    void connectNetwork();

        // originally in while loop with break
	    void sendText(Location value);
};

#endif