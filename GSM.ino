
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[] = "";
// initialize the library instance
GSM gsmAccess(true);
GSM_SMS sms;    // include a 'true' parameter to enable debugging
GSMScanner scannerNetworks;
GSMModem modem;
GSMClient client(false);
GPRS gprs;
GSMPIN PINManager;
boolean auth = false;

// APN data. Hologram Web Server does not use a login or password.
const char GPRS_APN[] = "prepay.pelion";
const char GPRS_LOGIN[] = "arduino";
const char GPRS_PASSWORD[] = "arduino";
String HOLOGRAM_API_KEY = "xxxxx";
bool clientConnected = false;     // Connection state flag
bool restartClient = true;        // Client restarted flag - initially set to ensure temperature is reported at startup
int unlockSimCard;       

// Save data variables
String IMEI = "";

// serial monitor result messages
String errortext = "ERROR";

void modemT() {
  // scan for existing networks, displays a list of networks
  Serial.println("Scanning available networks. May take some seconds.");
  Serial.println(scannerNetworks.readNetworks());

  // currently connected carrier
  Serial.print("Current carrier: ");
  Serial.println(scannerNetworks.getCurrentCarrier());

  // returns strength and ber
  // signal strength in 0-31 scale. 31 means power > 51dBm
  // BER is the Bit Error Rate. 0-7 scale. 99=not detectable
  Serial.print("Signal Strength: ");
  Serial.print(scannerNetworks.getSignalStrength());
  Serial.println(" [0-31]");

}



void setupGSM() {
  do 
  {
  } while (!startWebClient()); 
}

void sendGSM() {

  Serial.print("Enter a mobile number: ");
  char remoteNum[20];  // telephone number to send sms
  readSerial(remoteNum);
  Serial.println(remoteNum);

  // sms text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200];
  readSerial(txtMsg);
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");
}

/*
  Read input serial
 */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}
void testModem() {
  // get modem IMEI
  Serial.print("Checking IMEI...");
  IMEI = modem.getIMEI();

  // check IMEI response
  if (IMEI != NULL) {
    // show IMEI in serial monitor
    Serial.println("Modem's IMEI: " + IMEI);
    // reset modem to check booting:
    Serial.print("Resetting modem...");
    modem.begin();
    // get and check IMEI one more time
    if (modem.getIMEI() != NULL) {
      Serial.println("Modem is functoning properly");
    } else {
      Serial.println("Error: getIMEI() failed after modem.begin()");
    }
  } else {
    Serial.println("Error: Could not get IMEI");
  }
  // do nothing:
  while (true);
}

// Initialize GSM and GPRS
bool startWebClient()
{
  int  gsmBeginStatus;
  int  gprsAttachStatus;
  int  startWebClientInitializationCount = 0;
  int  gsmReadyStatus = 0;

  Serial.println("startWebClient HomeMonitorV5 Build 11-12-2018 Rev 1");

  // Initialize the GSM with a modem restart and asynchronous operation mode.
  // I modified the MODEM instance in the MKRGSM 1400 library to initialize with a baud rate of 115200.
  gsmBeginStatus = gsmAccess.begin(PINNUMBER, true, false);
  if (gsmBeginStatus == 0)
  {
    // Modem has been restarted. Delay for 2 seconds and loop while GSM component initializes and registers with network
    delay(4000);

    // March thru the GSM state machine one AT command at a time using the ready() method.
    // This allows us to detect a hang condition on the SARA U201 UART interface
    do
    {
      gsmReadyStatus = gsmAccess.ready();
      startWebClientInitializationCount++;
      delay(1000);       
    } while ((gsmReadyStatus == 0) && (startWebClientInitializationCount < 600));

    // If the GSM registered with the network attach to the GPRS network with the APN, login and password
    if (gsmReadyStatus == 1)
    {
      Serial.print("GSM registered successfully after ");
      Serial.print(startWebClientInitializationCount * 100);
      Serial.println(" ms");

      // Perform an asynchronous attach to the GPRS network.  That way we can prevent a GPRS hang in the MKRGSM1400 library
      gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD);
      do 
      {
        delay(1000);
        startWebClientInitializationCount++;
        Serial.println('GPRS: Try again...');
        gprsAttachStatus = gprs.ready();
      } while ((gprsAttachStatus == 0) && (startWebClientInitializationCount < 600));

      if (gprsAttachStatus == 1)
      {
        gprsAttachStatus = gprs.status();
        if (gprsAttachStatus == GPRS_READY)
        {
          Serial.println("Attached to APN");
          restartClient = true;
          return true;
        } else {
          Serial.print("GPRS status: ");
          Serial.println(gprsAttachStatus);
          return false;
        }
      } else if (gprsAttachStatus == 0) {      
        Serial.println();
        Serial.print("GPRS Attach timed OUT after ");
        Serial.print(startWebClientInitializationCount * 100);
        Serial.println(" ms");
        return false;
      } else {
        // Print gprsAttachStatus as ASCII encoded hex because occasionally we get garbage return characters when the network is in turmoil.
        // It appears the _ready variable in the MODEM Class instance is being overwritten with garbage.
        Serial.print("GPRS Attach status: ");
        Serial.println(gprsAttachStatus, HEX);
        return false;
      }
    } else if (gsmReadyStatus == 0) {
      Serial.print("GSM Ready status timed OUT after ");
      Serial.print(startWebClientInitializationCount * 100);
      Serial.println(" ms");
      return false;
    } else {
      // Print gsmReadyStatus as ASCII encoded hex because occasionally we get garbage return characters when the network is in turmoil.
      Serial.print("GSM Ready status: ");
      Serial.println(gsmReadyStatus, HEX);
      return false;
    }
  } else {
    Serial.print("GSM Begin status: ");
    Serial.println(gsmBeginStatus);
    return false;
  }
}
/*
void setupGSM() {
  // Initialize serial and wait up to 5 seconds for port to open 

  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }

  // This function takes care of connecting your sketch variables to the ArduinoIoTCloud object 
  initProperties();

  // Initialize Arduino IoT Cloud library 
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(DBG_INFO);
  ArduinoCloud.printDebugInfo();
}
*/

/*
  This sketch demonstrates how to use more complex cloud data types such as a colour or coordinates.

  This sketch is compatible with:
   - MKR 1000
   - MKR WIFI 1010
   - MKR GSM 1400
   - MKR NB 1500
   - MKR WAN 1300/1310
   - ESP 8266
*/
/*
void readGSM() {
//  ArduinoCloud.update();
}

void onSwitchButtonChange() {
  if (switchButton)
  {
    location = Location(latMov, lonMov);
    color    = Color(hueRed, satRed, briRed);
  }
  else
  {
    location = Location(latArd, lonArd);
    color    = Color(hueGreen, satGreen, briGreen);
  }
}

void onColorChange() {
  Serial.print("Hue = ");
  Serial.println(color.getValue().hue);
  Serial.print("Sat = ");
  Serial.println(color.getValue().sat);
  Serial.print("Bri = ");
  Serial.println(color.getValue().bri);
}

//=================================



void onSwitchButtonChange();
void onColorChange();

bool switchButton;
CloudLocation location;
CloudColor color;


 * void initProperties() {

  ArduinoCloud.setThingId(THING_ID);
  GSMConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);

  ArduinoCloud.addProperty(switchButton, WRITE, ON_CHANGE, onSwitchButtonChange);
  ArduinoCloud.addProperty(location, READ, ON_CHANGE);
  ArduinoCloud.addProperty(color, READWRITE, ON_CHANGE, onColorChange);

}
 * 
  */


// /*
//  SMS receiver

//  This sketch, for the MKR GSM 1400 board, waits for a SMS message
//  and displays it through the Serial port.

//  Circuit:
//  * MKR GSM 1400 board
//  * Antenna
//  * SIM card that can receive SMS messages

//  created 25 Feb 2012
//  by Javier Zorzano / TD
// ========================================




// */
// // include the GSM library
// #include <MKRGSM.h>

// #include "arduino_secrets.h" 
// // Please enter your sensitive data in the Secret tab or arduino_secrets.h
// // PIN Number
// const char PINNUMBER[] = SECRET_PINNUMBER;

// // initialize the library instances
// GSM gsmAccess;
// GSM_SMS sms;

// // Array to hold the number a SMS is retreived from
// char senderNumber[20];

// void setup() {
//   // initialize serial communications and wait for port to open:
//   Serial.begin(9600);
//   while (!Serial) {
//     ; // wait for serial port to connect. Needed for native USB port only
//   }

//   Serial.println("SMS Messages Receiver");

//   // connection state
//   bool connected = false;

//   // Start GSM connection
//   while (!connected) {
//     if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
//       connected = true;
//     } else {
//       Serial.println("Not connected");
//       delay(1000);
//     }
//   }

//   Serial.println("GSM initialized");
//   Serial.println("Waiting for messages");
// }

// void loop() {
//   int c;

//   // If there are any SMSs available()
//   if (sms.available()) {
//     Serial.println("Message received from:");

//     // Get remote number
//     sms.remoteNumber(senderNumber, 20);
//     Serial.println(senderNumber);

//     // An example of message disposal
//     // Any messages starting with # should be discarded
//     if (sms.peek() == '#') {
//       Serial.println("Discarded SMS");
//       sms.flush();
//     }

//     // Read message bytes and print them
//     while ((c = sms.read()) != -1) {
//       Serial.print((char)c);
//     }

//     Serial.println("\nEND OF MESSAGE");

//     // Delete message from modem memory
//     sms.flush();
//     Serial.println("MESSAGE DELETED");
//   }

//   delay(1000);

// }



// /*
//   Location

//   This sketch uses the celluar network to determine the location of a MKR GSM 1400 board
//   and prints it to the Serial monitor. The location is  based on the cellular towers in
//   range, and requires a GPRS data connection to be enabled.

//   Circuit:
//    MKR GSM 1400 board
//    Antenna
//    SIM card with a data plan

//   created 15 Dec 2017
//   by Sandeep Mistry





// */
// // libraries
// #include <MKRGSM.h>

// #include "arduino_secrets.h"
// // Please enter your sensitive data in the Secret tab or arduino_secrets.h
// // PIN Number
// const char PINNUMBER[]     = SECRET_PINNUMBER;
// // APN data
// const char GPRS_APN[]      = SECRET_GPRS_APN;
// const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
// const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

// // initialize the library instance
// GSMLocation location;
// GPRS gprs;
// GSM gsmAccess;

// void setup() {
//   // initialize serial communications and wait for port to open:
//   Serial.begin(9600);
//   while (!Serial) {
//     ; // wait for serial port to connect. Needed for native USB port only
//   }

//   Serial.println("Starting GSM location.");
//   // connection state
//   bool connected = false;

//   // After starting the modem with GSM.begin()
//   // attach the shield to the GPRS network with the APN, login and password
//   while (!connected) {
//     if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
//         (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
//       connected = true;
//     } else {
//       Serial.println("Not connected");
//       delay(1000);
//     }
//   }

//   location.begin();
// }

// void loop() {
//   if (location.available()) {
//     Serial.print("Location: ");
//     Serial.print(location.latitude(), 7);
//     Serial.print(", ");
//     Serial.println(location.longitude(), 7);

//     Serial.print("Altitude: ");
//     Serial.print(location.altitude());
//     Serial.println("m");

//     Serial.print("Accuracy: +/- ");
//     Serial.print(location.accuracy());
//     Serial.println("m");

//     Serial.println();
//   }
// }

// initialize the library instance
