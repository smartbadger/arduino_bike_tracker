#include <Arduino.h>
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
#define PN532_IRQ (6)
#define PN532_RESET (7) 

// TODO: define serial communication

int count = 0;
Indicator i = Indicator(greenLed, blueLed, redLed);
NfcReader nfc = NfcReader(PN532_IRQ, PN532_RESET, RFID_KEY);
Sensor s = Sensor();
sensors_event_t a, g,temp;
GSMInterface gsmi = GSMInterface(20000);
void setup() {
  // put your setup code here, to run once:
    {
    Serial.begin(115200);
    while (!Serial)
      delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }
  s.setup();
  nfc.setup();
  gsmi.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  i.setState(count);
  i.process();
  if(nfc.isAuthorized()){
    Serial.println("auth");
  };
  s.readSensor(a, g, temp);
  gsmi.doNetworkStuff();
  delay(2000);
  count++;
}