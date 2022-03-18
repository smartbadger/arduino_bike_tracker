#include "gsm_interface.h"
#include "secrets.h"

static const char *_PINNUMBER = SECRET_PIN;
static const char *_GPRS_APN = SECRET_APN;
static const char *_GPRS_LOGIN = SECRET_LOGIN;
static const char *_GPRS_PASSWORD = SECRET_PASS;

GSMInterface::GSMInterface(long timeout) {
	State _currentState = DISCONNECTED;
	State _targetState = READY;
	_timeout = timeout;
	_connected = false;
	_expired = false;

}

GSMInterface::~GSMInterface(){
	// destroy??
}

void GSMInterface::setup() {
	Serial.println("Setting up GSM Interface");
}

void GSMInterface::ready() {
	_targetState = _currentState;
}

void GSMInterface::doNetworkStuff()
{
	if (_targetState == DISCONNECTED)

	{
		goToReady();
	}
	else if (_targetState == READY)
	{
		goToReady();
	}
	else if (_targetState == SEND_STATUS)
	{
		goToSend();
	}
	
}
void GSMInterface::goToSend(){
	Serial.println("try to send");
}

void GSMInterface::goToDisconnected() {
	Serial.println("disconnect");
}

void GSMInterface::goToReady() {
	switch (_currentState)
    {
    case DISCONNECTED:
      connectNetwork();
      break;

    case SEND_STATUS:
      Serial.println("sending status");
      break;
    }
}

void GSMInterface::connect()
{
	connectNetwork();
	gsmlocation.begin();
}

// originally in while loop with break
// This function use the location's APIs to get the device coordinates and update the global variable if all the requirement are satisfied
Location GSMInterface::measureLocation()
{
	if (gsmlocation.available() && gsmlocation.accuracy() < 300 && gsmlocation.accuracy() != 0)
	{
		return Location
		{
			gsmlocation.latitude(),
			gsmlocation.longitude(),
			gsmlocation.altitude(),
			gsmlocation.accuracy()
		};
	}
	
		// negative accuracy means invalid
		Serial.println("location not available");
		return Location
		{ 0, 0, 0, -1 };
	
}

//The connectNetwork() function is used for the board data connection
// originally in while loop with break
void GSMInterface::connectNetwork()
{
	//set global AT command timeout this allow to recover from uart communication
	// freeze between samd module and ublox module.
	gprs.setTimeout(_timeout);
	gsmAccess.setTimeout(_timeout);
	long start = millis();
	// Start GSM connection
	while ( _currentState == DISCONNECTED || millis() - start < _timeout)
	{

		Serial.println("GSM: failed to connect");
		if ((gsmAccess.begin(_PINNUMBER) == GSM_READY) &(gprs.attachGPRS(_GPRS_APN, _GPRS_LOGIN, _GPRS_PASSWORD) == GPRS_READY))
		{
			_currentState = READY;
			Serial.println("GSM: connected to the network");
			ready();
		}

	}
}

void GSMInterface::setExpired(boolean value){
	this->_expired = value;
}

//code section used to initialize data connection and localization object
	//send json data to endpoint
void GSMInterface::sendData()
{

}

// originally in while loop with break
void GSMInterface::sendText(Location value)
{
	if (sms.available())
	{
		//check for SMS available
		char senderNumber[20] = { "0" };
		sms.remoteNumber(senderNumber, 20);	//Get remote number
		int c = sms.read();
		//Discard the the message if the first char is not a "L"
		if (c != 76)
		{
			sms.flush();
			// return;
		}

		//concatenate the string message to be sended to the remote number
		String txtMsg = "https://www.google.com/maps/place/" + String(value.latitude, 6) + "," + String(value.longitude, 6);

		// send the message
		sms.beginSMS(senderNumber);
		sms.print(txtMsg);
		sms.endSMS();
		// return;
	}
}
