#include <Arduino.h>

#include "models.h"
#include "debugger.h"
#include "config.h"
#include <map>

#include "ProcessManager.h"
#include "modules/sensor.h"
#include "modules/gsm_interface/gsm_interface.h"
#include "modules/nfcreader.h"
#include "modules/gps.h"
#include "utils.h"
#include "StateMachine/StateMachine.h"
#include <memory>

using namespace GPS;
using namespace GYRO;
using namespace NFC;
using namespace System;

ProcessManager processM;
GSMInterface GsmController = GSMInterface();

std::unique_ptr<State> state(new Locked()); // State *state = new Locked(); <-- similar but not as safe

//=================================================================================================
// Helper Functions
//=================================================================================================
bool printBike(void *)
{
  BikeData data = state->getBikeData();
  state->printBikeData();
  return true;
  // return false;
}
bool callGSM(void *)
{
  GsmController.doNetworkStuff(); // can take some time
  return true;
  // return false;
}

void onWakeUp()
{
  debuglnV("Woke up from sleep");
}

bool callNFC(void *)
{
  bool auth = NFC::isAuthorized();
  if (NFC::success && auth)
  {
    // TODO: setup debounce
    state = state->nfc_authenticated();
  }
  else if (NFC::success && !auth)
  {
    state = state->nfc_rejected();
  }
  return true;
  // return false;
}
bool readSensor(void *)
{
  BikeData oldData = state->getBikeData();
  GYRO::readSensor(*state);
  const BikeData &newData = state->getBikeData(); // at runtime don't keep copying
  if (sensorHasChangedWithThresholds(oldData.g, newData.g, 1) || sensorHasChangedWithThresholds(oldData.a, newData.a, 1))
  {
    state = state->motion_detected();
  }

  return true;
  // return false;
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

  processM.every(120000, printBike);
  processM.every(5000, callNFC);
  processM.every(1000, readSensor);
  // processM.every(30000, callGSM);
  processM.start();
}

void loop()
{

  // this doesn't seem to work
  // processM.in(120000, printBike);
  // processM.in(5000, callNFC);
  // processM.in(1000, readSensor);
  // processM.in(30000, callGSM);

  GPS::GPSloop(*state);
  processM.update();
  // state->update();
}

// TODO: implement low power, silent mode, check in frequency, and hardware removal
// TODO: add watchdog for reset, (ideally make a countdown that needs to be refreshed incase code fails)
