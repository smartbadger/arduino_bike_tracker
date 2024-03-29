#include "gsm_interface.h"

static const char *_PINNUMBER = SECRET_PIN;
static const char *_GPRS_APN = SECRET_APN;
static const char *_GPRS_LOGIN = SECRET_LOGIN;
static const char *_GPRS_PASSWORD = SECRET_PASS;
static const char *_SERVER = SERVER_URL;
static const int _SERVER_PORT = SERVER_PORT;

GSMInterface::GSMInterface()
{
	_currentState = DISCONNECTED;
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
	_modemReady = true;
	return;
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

void GSMInterface::doNetworkStuff(State &state)
{
	if (!_modemReady)
	{
		setup();
	}
	else if (_currentState == CONNECTING)
	{
		if (gsmAccess.ready() == 1)
		{
			debuglnV("GSM: connected to the network");
			_currentState = READY;
			_connected = true;
		}
		// else if (gsmAccess.ready() >= 2)
		// {
		// 	debuglnV("GSM: not connected to the network");
		// 	_currentState = DISCONNECTED;
		// 	_connected = false;
		// }
		else
		{
			debuglnV("GSM: trying to connect to the network");
			// Serial.println("Scanning available networks. May take some seconds.");
			// Serial.println(scanner.readNetworks());

			// // currently connected carrier
			Serial.print("Current carrier: ");
			Serial.print(scanner.getCurrentCarrier());

			// // returns strength and ber
			// // signal strength in 0-31 scale. 31 means power > 51dBm
			// // BER is the Bit Error Rate. 0-7 scale. 99=not detectable
			Serial.print("Signal Strength: ");
			Serial.print(scanner.getSignalStrength());
			Serial.println(" [0-31]");
		}
	}
	else if (_currentState == DISCONNECTED)
	{
		connectNetwork();
	}
	else if (_currentState == READY)
	{
		measureLocation();
		state.setGSM(false, getNetworkStatus());
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
	// gprs.setTimeout(_timeout);
	// gsmAccess.setTimeout(_timeout);
	// long start = millis();

	debuglnV("Attempting connection...");
	gsmAccess.begin(_PINNUMBER, true, false) &&
		gprs.attachGPRS(SECRET_APN, SECRET_LOGIN, SECRET_PASS);
	_currentState = CONNECTING;
}
String GSMInterface::getNetworkStatus()
{
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