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
#include "sensor.h"
#include "sample.h"

#define PN532_IRQ (6)
#define PN532_RESET (7) // Not connected by default on the NFC Shield

#define blueLed (5)
#define greenLed (4)
#define redLed (3)

#define alarm (2)

// TODO: define serial communication
#define THING_ID "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
#define BOARD_ID "615b3b99-01d6-4966-9daa-35a6a3c61a6e"

#define sfPrint(x) Serial.println(F(x)); // to disable debugging comment
// #define sfPrint(x); // to disable debugging uncomment

long runningTime = millis(); // time since startup to subtract for internal clock
bool debug = true;           // set to false for production

// setup bike class
NfcReader nfc = NfcReader(PN532_IRQ, PN532_RESET);
Sensor sen = Sensor();
Bike bike = Bike();
// GsmModule gsm = GSM();

bool aboveThresholdValue(float original, float current, float threshold)
{
  return abs((original - current) / original) >= threshold;
}

bool checkForMotion(Sample current, Sample previous)
{
  float t = 0.5;
  float cr = current.rotation.calculateAverage();
  float pr = previous.rotation.calculateAverage();
  float ca = current.acceleration.calculateAverage();
  float pa = previous.acceleration.calculateAverage();
  return aboveThresholdValue(cr, pr, t) || aboveThresholdValue(ca, pa, t);
}

void setup()
{
  if (debug)
  {
    Serial.begin(115200);
    while (!Serial)
      delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  nfc.setupNFC();
  sen.setup();
}

void loop()
{
  // get time elapsed, replaces delay() for 'hyper-threading'
  long elapsedTime = millis() - runningTime;
  static bool motionTrigger = false;
  runningTime += elapsedTime;

  bool auth = nfc.nfcAuthentication(runningTime);
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
  Sample s = sen.readSensor(elapsedTime);
  bool motion = checkForMotion(bike.getSample(), s);
  bike.setSample(s);
  
}