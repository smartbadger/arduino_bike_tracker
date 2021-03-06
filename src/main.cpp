#include "nfcreader.h"
#include <Arduino.h>
#include <arduino-timer.h>
#include "ArduinoLowPower.h"
#include "indicator.h"
#include "sensor.h"
#include "gsm_interface.h"
#include "secrets.h"
#include "bikestate.h"
#include "debugger.h"

#define blueLed (5)
#define greenLed (4)
#define redLed (3)
#define alarm (2)
#define blink (1)
#define power (0)
#define PN532_IRQ (6)
#define PN532_RESET (7) 
#define debugging true

Indicator IND = Indicator(blink, greenLed, blueLed, redLed, alarm, power);
NfcReader NFC = NfcReader(PN532_IRQ, PN532_RESET, RFID_KEY);
Sensor SEN = Sensor();
GSMInterface GSMI = GSMInterface(20000);

bikedata bike;
sensors_event_t a, g,temp;
int counter = 0;
auto t1 = timer_create_default();
auto t2 = timer_create_default();
auto t3 = timer_create_default();
auto t4 = timer_create_default();

// TODO: implement low power, silent mode, check in frequency, and peripheral/sensor disrupts
// TODO: add watchdog for reset, (ideally make a countdown that needs to be refreshed incase code fails)
bool printBike(void *){
  bike.print();
  return true;
}

void deepSleep(int timeout) {

  debuglnV("inactive setting sleep...");
  counter = 0;
  
  // look into deep sleep with external wake up 
  // PN532 Sleep somewhere here...
  SEN._mpu.enableSleep(true); // MPU6050 Sleep
  IND.sleep(); // Lights OFF
  LowPower.sleep(timeout);
}

float readBattery(bool volt) { // Read battery voltage
  int sensorValue = analogRead(ADC_BATTERY);
  if (volt) {
    return sensorValue * (4.2 / 1023.0);
  }
  else {
    return sensorValue;
  }
}

bool readSensor(void *){
    debuglnV("read Sensor");
  SEN.readSensor(&bike);
  bike.battery = readBattery(true);
  if(bike.motion){
    counter = 0;
  }else {
    counter ++;
  }
  if (bike.locked && bike.motion){
    IND.setState(3);
    bike.motion = false;
  }
  return true;
}

bool callGSM(void *){
  debuglnV("calling GSM");
  auto state = IND.getState();
  IND.setState(4);
  IND.update();
  GSMI.doNetworkStuff(&bike); // can take some time
  IND.setState(state);
  IND.update();
  // revert to prev state
  return true;
}

bool callNFC(void *){
  debuglnV("calling NFC");
  bool auth = NFC.isAuthorized();
  if(auth && bike.locked){
    IND.setState(2);
    bike.locked = false;
  }else if (auth && !bike.locked){
    IND.setState(1);
    bike.locked = true;
  }
  IND.update();
  return true;
}

void onWakeUp() {
  debuglnV("WOKE UP");
  SEN._mpu.enableSleep(false);
  IND.update();
}

void setup(void) {
  // put your setup code here, to run once:    
  Serial.begin(115200);
  if(debugging){
    while (!Serial){
      delay(100); // will pause Zero, Leonardo, etc until serial console opens
    }
  }
  LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, onWakeUp, CHANGE);
  SEN.setup();
  NFC.setup();
  GSMI.setup();
  IND.setup();
  // print state every 10s 
  t3.every(10000, printBike);

}
void loop(void) {
  if (t1.empty()) {
    t1.in(30000, callGSM);
  }
  if (t2.empty()) {
    t2.in(2000, callNFC);
  }
  if (t4.empty()) {
    t4.in(500, readSensor);
  }
  if(counter > 30){
    deepSleep(10000);
  }
  t1.tick();
  t2.tick();
  t3.tick();
  t4.tick();
  IND.update();
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