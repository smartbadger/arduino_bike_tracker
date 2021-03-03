
class GSMInterface {

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[] = "";
// initialize the library instance
GSM gsmAccess(true);
GSM_SMS sms; // include a 'true' parameter to enable debugging
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
bool clientConnected = false; // Connection state flag
bool restartClient = true;    // Client restarted flag - initially set to ensure temperature is reported at startup
int unlockSimCard;

// Save data variables
String IMEI = "";

// serial monitor result messages
String errortext = "ERROR";

void modemT(long elapsedTime)
{
  // check for motion if the module is available otherwise try to ping it
  static long sensorReadTime = 0; // interval to read nfc tag at
  int readInterval = 60000;

  sensorReadTime += elapsedTime;
  if (sensorReadTime >= readInterval)
  {
    sensorReadTime -= readInterval;
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
}

void setupGSM()
{
  startWebClient(-1);
}

void testModem()
{
  // get modem IMEI
  Serial.print("Checking IMEI...");
  IMEI = modem.getIMEI();

  // check IMEI response
  if (IMEI != NULL)
  {
    // show IMEI in serial monitor
    Serial.println("Modem's IMEI: " + IMEI);
    // reset modem to check booting:
    Serial.print("Resetting modem...");
    modem.begin();
    // get and check IMEI one more time
    if (modem.getIMEI() != NULL)
    {
      Serial.println("Modem is functoning properly");
    }
    else
    {
      Serial.println("Error: getIMEI() failed after modem.begin()");
    }
  }
  else
  {
    Serial.println("Error: Could not get IMEI");
  }
}

// Initialize GSM and GPRS
bool startWebClient(long elapsedTime)
{
  sfPrint("startWebClient HomeMonitorV5 Build 11-12-2018 Rev 1");
  static int gsmBeginStatus = -1;
  static int gprsAttachStatus = -1;
  static int startWebClientInitializationCount = 0;
  static int gsmReadyStatus = 0;

  static long sensorReadTime = 0; // interval to read nfc tag at
  int readInterval = 5000;
  // Initialize the GSM with a modem restart and asynchronous operation mode.
  // I modified the MODEM instance in the MKRGSM 1400 library to initialize with a baud rate of 115200.
  if (sensorReadTime >= readInterval)
  {
    gsmBeginStatus = gsmAccess.begin(PINNUMBER, true, false);
    if (gsmBeginStatus == 0)
    {
      // Modem has been restarted. Delay for 2 seconds and loop while GSM component initializes and registers with network
      sensorReadTime += elapsedTime;
      if (sensorReadTime >= readInterval)
      {
        sensorReadTime -= readInterval;
      }

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
          }
          else
          {
            Serial.print("GPRS status: ");
            Serial.println(gprsAttachStatus);
            return false;
          }
        }
        else if (gprsAttachStatus == 0)
        {
          Serial.println();
          Serial.print("GPRS Attach timed OUT after ");
          Serial.print(startWebClientInitializationCount * 100);
          Serial.println(" ms");
          return false;
        }
        else
        {
          // Print gprsAttachStatus as ASCII encoded hex because occasionally we get garbage return characters when the network is in turmoil.
          // It appears the _ready variable in the MODEM Class instance is being overwritten with garbage.
          Serial.print("GPRS Attach status: ");
          Serial.println(gprsAttachStatus, HEX);
          return false;
        }
      }
      else if (gsmReadyStatus == 0)
      {
        Serial.print("GSM Ready status timed OUT after ");
        Serial.print(startWebClientInitializationCount * 100);
        Serial.println(" ms");
        return false;
      }
      else
      {
        // Print gsmReadyStatus as ASCII encoded hex because occasionally we get garbage return characters when the network is in turmoil.
        Serial.print("GSM Ready status: ");
        Serial.println(gsmReadyStatus, HEX);
        return false;
      }
    }
    else
    {
      Serial.print("GSM Begin status: ");
      Serial.println(gsmBeginStatus);
      return false;
    }
  }
  return false;
}
};