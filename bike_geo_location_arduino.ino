// GLOBAL
#include <Wire.h>
#include <SPI.h>
// #include <Adafruit_Sensor.h>
// #include <ArduinoIoTCloud.h>
// #include <Arduino_ConnectionHandler.h>
// #include <MKRGSM.h>
// #include <GSM.h>
// #include <Async_Operations.h>
#include "secrets.h"
#include "bike.h"
#include "nfc_reader.h"
#include "sensordata.h"
#include "Async_Operations.h"
#include "indicator.h"
#include "timer.h"

#define PN532_IRQ (6)
#define PN532_RESET (7) // Not connected by default on the NFC Shield

// #define blueLed (5)
// #define greenLed (4)
// #define redLed (3)
#define blueLed (5)
#define greenLed (4)
#define redLed (3)

#define alarm (2)

// TODO: define serial communication
#define THING_ID "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
#define BOARD_ID "615b3b99-01d6-4966-9daa-35a6a3c61a6e"

// #define sfPrint(x); // to disable debugging uncomment

bool debug = true; // set to false for production


long long dtt = {250};
Async_Operations dauth(&dtt, 1, -1);

// setup bike class
Sensor sensor = Sensor();
NfcReader nfc = NfcReader(PN532_IRQ, PN532_RESET);
Indicator status = Indicator(greenLed, blueLed, redLed, alarm);
Bike bike = Bike(sensor);

// GsmModule gsm = GSM();

void setup()
{
  if (debug)
  {
    Serial.begin(115200);
    while (!Serial)
      delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  bike.setup();
  nfc.setupNFC();
  dauth.start();
}

void readNFC()
{
  bool auth = nfc.nfcAuthentication();
  bool locked = bike.isLocked();
  // put your main code here, to run repeatedly:
  if (locked && auth)
  {
    bike.setLock(false);
    Serial.println("set unlocked");
  }
  else if (!locked && auth)
  {
    bike.setLock(true);
    Serial.println("set locked");
  }
}

void loop()
{
}