#ifndef GSM_INTERFACE_H
#define GSM_INTERFACE_H

#include <MKRGSM.h>
#include "ArduinoLowPower.h"
#include "location.h"

class GSMInterface
{

	public:
		GSMInterface(long timeout);
        ~GSMInterface();
	    void ready();
	    void loop();
        void setup();

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
        // originally in while loop with break
	    // This function use the location's APIs to get the device coordinates and update the globa variable if all the requirement are satisfied
	    Location measureLocation();

	    //The connectNetwork() function is used for the board data connection
        // originally in while loop with break
	    void connectNetwork();
	

	    //code section used to initialize data connection and localization object
		//send json data to endpoint
    	void sendData();

        // originally in while loop with break
	    void sendText(Location value);
};

#endif