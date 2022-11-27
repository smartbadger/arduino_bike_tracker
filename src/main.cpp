#include <Arduino.h>

#include "models.h"
#include "debugger.h"
#include "config.h"
#include <map>

#include "ProcessManager.h"
#include "modules/sensor/sensor.h"
#include "modules/gsm_interface/gsm_interface.h"
#include "modules/nfcreader/nfcreader.h"
#include "modules/gps/gps.h"
#include "system/system.h"
#include "observer.h"

BikeDataObservable bike;
EventObserver obs;
int counter = 0;

// TODO: implement low power, silent mode, check in frequency, and hardware removal
// TODO: add watchdog for reset, (ideally make a countdown that needs to be refreshed incase code fails)

using namespace GPS;
using namespace GYRO;
using namespace NFC;
using namespace System;

GSMInterface GsmController = GSMInterface();
ProcessManager processM;

//=================================================================================================
// Helper Functions
//=================================================================================================
bool printBike(void *)
{
  bike.print();
  return true;
}
bool callGSM(void *)
{

  debuglnV("calling GSM");
  GsmController.doNetworkStuff(&bike); // can take some time

  return false;
}

void onWakeUp()
{
  debuglnV("Woke up from sleep");
}

bool callNFC(void *)
{
  debuglnV("calling NFC");
  bool auth = NFC::isAuthorized();
  if (NFC::success && auth)
  {
    // TODO setup debounce
    bike.toggleLocked();
  }

  return false;
}
bool readSensor(void *)
{
  debuglnV("read Sensor");
  GYRO::readSensor(&bike);
  return false;
}

//=================================================================================================
// Setup and Main Loop
//=================================================================================================
void setup(void)
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (DEBUGLEVEL)
  {
    while (!Serial)
    {
      delay(100); // will pause Zero, Leonardo, etc until serial console opens
    }
  }

  System::setup(onWakeUp);
  GPS::setup();
  NFC::setup();
  bike.subscribe(obs);

  processM.every(1000, printBike);
  processM.every(5000, callNFC);
  processM.every(1000, readSensor);
  processM.every(30000, callGSM);
  processM.start();
}
void loop(void)
{

  GPS::GPSloop(bike);
  processM.update();
}

// Additional notes
//========================

// CONDITION: when bike is unlocked and moving

// GSM call frequency low
// NFC calls call frequency very low
// sensor call frequency low

// CONDITION: when bike is locked not moving
// GSM call frequency very low
// NFC calls call frequency very low
// sensor call frequency medium

// CONDITION: when bike is locked and moving
// GSM call frequency high
// NFC calls call frequency low
// sensor call frequency medium/low
// alarm call medium/high

// CONDITION: when bike is locked and moving low battery
// GSM call frequency low / sleep
// NFC calls call frequency very low / sleep
// sensor call frequency very low / sleep

// CONDITION: when bike is unlocked and idle for a set amount of time
// lock bike
