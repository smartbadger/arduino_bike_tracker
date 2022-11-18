#include "gsm_interface.h"

static const char *_PINNUMBER = SECRET_PIN;
static const char *_GPRS_APN = SECRET_APN;
static const char *_GPRS_LOGIN = SECRET_LOGIN;
static const char *_GPRS_PASSWORD = SECRET_PASS;
static const char *_SERVER = SERVER_URL;
static const int _SERVER_PORT = SERVER_PORT;

GSMInterface::GSMInterface()
{
	State _currentState = DISCONNECTED;
	_timeout = GSM_TIMEOUT;
	_connected = false;
	_expired = false;
	_modemReady = false;
}

GSMInterface::~GSMInterface()
{
	// destroy??
}

void GSMInterface::setup()
{
	debuglnV("Setting up GSM Interface");
	long start = millis();
	while (!_modemReady && millis() - start < _timeout)
	{
		if (!scanner.begin())
		{
			debuglnV("Issue connecting to modem");
			delay(1000);
		}
		else
		{
			_modemReady = true;
			debuglnV("Modem OK");
			break;
		}
	}
}

void GSMInterface::doNetworkStuff(bikedata *data)
{
	if (_currentState == DISCONNECTED)
	{
		connectNetwork();
	}
	else if (_currentState == READY)
	{
		// data->location = measureLocation();
		data->signal = getNetworkStatus();
	}
}

// originally in while loop with break
// This function use the location's APIs to get the device coordinates and update the global variable if all the requirement are satisfied
Location GSMInterface::measureLocation()
{
	if (gsmlocation.available() && gsmlocation.accuracy() != 0)
	{
		debuglnV("location available");
		return Location{
			gsmlocation.latitude(),
			gsmlocation.longitude(),
			gsmlocation.altitude(),
			gsmlocation.accuracy()};
	}
	// negative accuracy means invalid
	debuglnV("location not available");
	return Location{0, 0, 0, -1};
}

// The connectNetwork() function is used for the board data connection
//  originally in while loop with break
void GSMInterface::connectNetwork()
{
	// set global AT command timeout this allow to recover from uart communication
	//  freeze between samd module and ublox module.
	gprs.setTimeout(_timeout);
	gsmAccess.setTimeout(_timeout);
	long start = millis();
	debuglnV("Attempting connection...");
	// Start GSM connection
	while (_currentState == DISCONNECTED && millis() - start < _timeout)
	{
		if ((gsmAccess.begin(_PINNUMBER) == GSM_READY) & (gprs.attachGPRS(_GPRS_APN, _GPRS_LOGIN, _GPRS_PASSWORD) == GPRS_READY))
		{
			debuglnV("GSM: connected to the network");
			_currentState = READY;
			_connected = true;
			break;
		}
		else
		{
			debuglnV("GSM: failed to connect");
		}
	}
}
String GSMInterface::getNetworkStatus()
{
	// debuglnV(scanner.readNetworks());
	// scanner.getCurrentCarrier();
	return scanner.getSignalStrength();
}
void GSMInterface::setExpired(boolean value)
{
	this->_expired = value;
}

// code section used to initialize data connection and localization object
// send json data to endpoint
void GSMInterface::sendDataFromClient()
{
	// if you get a connection, report back via serial:
	if (client.connect(_SERVER, _SERVER_PORT))
	{
		debuglnV("connected");
		// Send a Message request:
		// client.println("{\"k\":\"" + HOLOGRAM_DEVICE_KEY +"\",\"d\":\""+ HOLOGRAM_MESSAGE+ "\",\"t\":\""+HOLOGRAM_TOPIC+"\"}");
	}
	else
	{
		// if you didn't get a connection to the server:
		debuglnV("connection failed");
	}
}
void GSMInterface::readDataFromServer()
{
	if (client.available())
	{
		char c = client.read();
		Serial.print(c);
	}
}
void GSMInterface::readSMS()
{
}
// originally in while loop with break
void GSMInterface::sendSMS()
{
	if (sms.available())
	{
		// check for SMS available
		//  char senderNumber[20] = { "0" };
		//  sms.remoteNumber(senderNumber, 20);	//Get remote number
		//  int c = sms.read();
		//  //Discard the the message if the first char is not a "L"
		//  if (c != 76)
		//  {
		//  	sms.flush();
		//  	// return;
		//  }

		// //concatenate the string message to be sended to the remote number
		// String txtMsg = "https://www.google.com/maps/place/" + String(value.latitude, 6) + "," + String(value.longitude, 6);

		// // send the message
		// sms.beginSMS(senderNumber);
		// sms.print(txtMsg);
		// sms.endSMS();
		// return;
	}
	else
	{
		debuglnV("error sending");
	}
}