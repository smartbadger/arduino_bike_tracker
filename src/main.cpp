#include <Arduino.h>
#include <arduino-timer.h>
#include "debugger.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "indicator.h"
#include "nfcreader.h"
#include "sensor.h"
#include "gsm_interface.h"
#include "secrets.h"
#include "bikestate.h"

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

sensors_event_t a, g,temp;
auto t1 = timer_create_default();
auto t2 = timer_create_default();
auto t3 = timer_create_default();
auto t4 = timer_create_default();
bikedata bike;

bool printBike(void *){
  bike.print();
  return true;
}

bool readSensor(void *){
    debuglnV("read Sensor");
  SEN.readSensor(&bike);
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
  IND.process();
  GSMI.doNetworkStuff(&bike); // can take some time
  IND.setState(state);
  IND.process();
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
  IND.process();
  return true;
}

void setup() {
  // put your setup code here, to run once:    
  Serial.begin(115200);
  if(debugging){
    while (!Serial){
      delay(100); // will pause Zero, Leonardo, etc until serial console opens
    }
  }

  SEN.setup();
  NFC.setup();
  GSMI.setup();
  IND.setState(1);
  IND.process();
  t3.every(10000, printBike);
}
void loop() {
  if (t1.empty()) {
    t1.in(30000, callGSM);
  }
  if (t2.empty()) {
    t2.in(2000, callNFC);
  }
  if (t4.empty()) {
    t4.in(500, readSensor);
  }
  t1.tick();
  t2.tick();
  t3.tick();
  t4.tick();
  IND.process();
}