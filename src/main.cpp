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

#define blueLed (5)
#define greenLed (4)
#define redLed (3)
#define alarm (2)
#define blink (1)
#define relay (0)
#define PN532_IRQ (6)
#define PN532_RESET (7) 

Indicator IND = Indicator(blink, greenLed, blueLed, redLed, alarm);
NfcReader NFC = NfcReader(PN532_IRQ, PN532_RESET, RFID_KEY);
Sensor SEN = Sensor();
GSMInterface GSMI = GSMInterface(20000);

sensors_event_t a, g,temp;
auto timer = timer_create_default();
bool locked = true;
bool auth = false;
bool motion = false;

bool callGSM(void *){
  IND.setState(4);
  IND.process();
  GSMI.doNetworkStuff();
  // revert to prev state
  return true;
}

bool callNFC(void *){
  auth = NFC.isAuthorized();
  return true;
}

bool callMotion(void *){
  motion = SEN.readSensor(a, g, temp);
  return true;
}

void setup() {
  // put your setup code here, to run once:    
  Serial.begin(115200);
  while (!Serial){
    delay(100); // will pause Zero, Leonardo, etc until serial console opens
  }

  SEN.setup();
  NFC.setup();
  GSMI.setup();
  timer.every(30000, callGSM);
  timer.every(2000, callNFC);
  timer.every(4000, callMotion);
}
void loop() {
  // put your main code here, to run repeatedly:
  timer.tick();
  if(auth && locked){
    IND.setState(2);
    locked = false;
  }else if (locked && motion){
    IND.setState(3);
    motion = false;
  }else if (auth && !locked){
    IND.setState(1);
    locked = true;
  }else if(!locked) {
    IND.setState(2);
  }else {
    IND.setState(1);
  }
  IND.process();

}