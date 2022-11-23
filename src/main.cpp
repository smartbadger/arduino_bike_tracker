#include <Arduino.h>
#include <arduino-timer.h>
#include "models.h"
#include "debugger.h"
#include "config.h"
#include <map>

#include "modules/sensor/sensor.h"
#include "modules/gsm_interface/gsm_interface.h"
#include "observer.h"
#include "modules/nfcreader/nfcreader.h"
#include "modules/gps/gps.h"
#include "system/system.h"

BikeDataObservable bike;
EventObserver obs;
int counter = 0;

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

// GSMInterface GsmController = GSMInterface();
typedef bool (*FuncPtrBoolInt)(void *);
struct ProcessManager
{
  Timer<10> timer;
  std::map<bool (*)(void *), Timer<10>::Task> processes;

  enum TaskState
  {
    PAUSED,
    RUNNING,
  };

  TaskState currentState = PAUSED;
  bool onComplete(bool (*cb)(void *))
  {
    debuglnV("Process Complete");
    bool repeat = cb(nullptr);
    if (!repeat)
    {
      processes.erase(cb);
    }
    return repeat;
  }
  void in(unsigned long delay, bool (*cb)(void *))
  {
    // q: how to cast to bool (*)(void *) to avoid compiler warning
    Timer<10>::Task task = timer.in(delay, cb);
    addProcess(cb, task);
  }
  void at(unsigned long delay, bool (*cb)(void *))
  {
    Timer<10>::Task task = timer.at(delay, cb);
    addProcess(cb, task);
  }
  void every(unsigned long delay, bool (*cb)(void *))
  {
    Timer<10>::Task task = timer.every(delay, cb);
    addProcess(cb, task);
  }
  void addProcess(bool (*cb)(void *), Timer<10>::Task task)
  {

    if (processes.find(cb) != processes.end())
    {
      debuglnV("Process already exists");
      timer.cancel(task);
    }
    else
    {
      processes[cb] = task;
    }
  }
  void remove(bool (*cb)(void *))
  {
    if (processes.find(cb) != processes.end())
    {
      timer.cancel(processes[cb]);
      processes.erase(cb);
    }
  }

  void start()
  {
    currentState = RUNNING;
  }
  void pause()
  {
    currentState = PAUSED;
  }
  void update()
  {
    if (currentState == RUNNING)
    {
      timer.tick();
    }
  }
};

ProcessManager processM;

bool callGSM(void *)
{

  debuglnV("calling GSM");
  // GsmController.doNetworkStuff(&bike); // can take some time
  return false;
}

// debuglnV("inactive setting sleep for 1min");
// look into deep sleep with external wake up
// PN532 Sleep somewhere here...
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
