// GLOBAL 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "secrets.h"


#define PN532_IRQ   (6)
#define PN532_RESET (7)  // Not connected by default on the NFC Shield

#define blueLed (5)
#define greenLed (4)
#define redLed (3)

#define alarm (2)

// TODO: define serial communication
#define THING_ID "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
#define BOARD_ID "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
Adafruit_MPU6050 mpu;

/* 
 *  0 = ready / unlocked
 *  1 = locked 
 *  2 = active 
 *  3 = active tracking
 *  4 = active low power
 *  
 */
int bikeState = 1;

float xRot, yRot, zRot;
float xAcc, yAcc, zAcc;
String sensorTemp;

float latMov = 45.5058224, lonMov = 9.1628673;
float latArd = 45.0502078, lonArd = 7.6674765;

float hueRed = 0.0, satRed = 100.0, briRed = 100.0;
float hueGreen = 80.0, satGreen = 100.0, briGreen = 100.0;

long runningTime = millis(); // time since startup to subtract for internal clock
