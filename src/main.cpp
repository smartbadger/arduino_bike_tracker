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
#define relay (0)
#define PN532_IRQ (6)
#define PN532_RESET (7) 
#define debugging true

Indicator IND = Indicator(blink, greenLed, blueLed, redLed, alarm);
NfcReader NFC = NfcReader(PN532_IRQ, PN532_RESET, RFID_KEY);
Sensor SEN = Sensor();
GSMInterface GSMI = GSMInterface(20000);

sensors_event_t a, g,temp;
auto t1 = timer_create_default();
auto t2 = timer_create_default();
auto t3 = timer_create_default();
bool locked = true;
bool motion = false;
bikedata bike;

bool printBike(void *){
  Serial.println("BIKE STATE:");
  Serial.print("Locked:");
  Serial.print(bike.locked);
  Serial.println("");
  Serial.print("Signal:");
  Serial.print(bike.signal);
  Serial.println("LOCATION");
  Serial.print("latitude:");
  Serial.print(bike.location.latitude);
  Serial.print("  longitude:");
  Serial.print(bike.location.longitude);
  Serial.print("  accuracy:");
  Serial.print(bike.location.accuracy);
  Serial.print("  altitude:");
  Serial.print(bike.location.altitude);
  Serial.println("SENSOR");
  Serial.print("gyroX:");
  Serial.print(bike.g.gyro.heading);
  Serial.println("");
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
  if(auth && locked){
    IND.setState(2);
    locked = false;
  }else if (auth && !locked){
    IND.setState(1);
    locked = true;
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
  t1.tick();
  t2.tick();
  t3.tick();

  motion = SEN.readSensor(bike.a, bike.g, bike.temp);
  if (locked && motion){
    IND.setState(3);
    motion = false;
    delay(1000);
  }

  IND.process();
}