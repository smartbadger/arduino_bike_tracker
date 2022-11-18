#include <Arduino.h>
#include <arduino-timer.h>
#include "models.h"
#include "debugger.h"
#include "config.h"

#include "modules/sensor/sensor.h"
#include "modules/gsm_interface/gsm_interface.h"
#include "modules/nfcreader/nfcreader.h"
#include "modules/gps/gps.h"
#include "system/system.h"

using namespace GPS;
using namespace GYRO;
using namespace NFC;

bikedata bike;
sensors_event_t a, g, temp;
int counter = 0;
auto t1 = timer_create_default();
auto t2 = timer_create_default();
auto t3 = timer_create_default();
auto t4 = timer_create_default();

// TODO: implement low power, silent mode, check in frequency, and hardware removal
// TODO: add watchdog for reset, (ideally make a countdown that needs to be refreshed incase code fails)
bool printBike(void *)
{
  bike.print();
  return true;
}

using namespace GPS;
using namespace GYRO;
using namespace NFC;
using namespace System;

// debuglnV("inactive setting sleep for 1min");
// look into deep sleep with external wake up
// PN532 Sleep somewhere here...
void onWakeUp()
{
  debuglnV("Woke up from sleep");
}
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
}
void loop(void)
{
  // if (t1.empty())
  // {
  //   t1.in(30000, callGSM);
  // }
  // if (t2.empty())
  // {
  //   t2.in(2000, callNFC);
  // }
  // if (t4.empty())
  // {
  //   t4.in(500, readSensor);
  // }
  // if (counter > 30)
  // {
  //   deepSleep();
  // }
  // t1.tick();
  // t2.tick();
  // t3.tick();
  // t4.tick();
  // IND.process();
  // useGPS::GPSloop(bike);
}

// bool readSensor(void *)
// {
//   debuglnV("read Sensor");
//   gyrosensor::readSensor(&bike);
//   bike.battery = readBattery(true);
//   if (bike.motion)
//   {
//     counter = 0;
//   }
//   else
//   {
//     counter++;
//   }
//   if (bike.locked && bike.motion)
//   {
//     IND.setState(3);
//     bike.motion = false;
//   }
//   return true;
// }

// bool callGSM(void *)
// {
//   debuglnV("calling GSM");
//   auto state = IND.getState();
//   IND.setState(4);
//   IND.process();
//   GSMI.doNetworkStuff(&bike); // can take some time
//   IND.setState(state);
//   IND.process();
//   // revert to prev state
//   return true;
// }

// bool callNFC(void *)
// {
//   debuglnV("calling NFC");
//   bool auth = nfcReaderpn::isAuthorized();
//   if (auth && bike.locked)
//   {
//     IND.setState(2);
//     bike.locked = false;
//   }
//   else if (auth && !bike.locked)
//   {
//     IND.setState(1);
//     bike.locked = true;
//   }
//   IND.process();
//   return true;
// }

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
