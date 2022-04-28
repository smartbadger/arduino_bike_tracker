#pragma once
#ifndef BIKESTATE_H
#define BIKESTATE_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
// #include "debugger.h"
#include "location.h"

struct bikedata {
  bool locked = true;
  bool motion = false;
  bool powerOn = false;
  double battery = 0.0;
  bool network = false;
  Location location = {0, 0, 0, -1};
  String signal = "0";
  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t temp;
  void print() {
    String loc = "  latitude: " + String(location.latitude) + ", longitude: " + String(location.longitude) + ",  accuracy: " + String(location.accuracy) + ", altitude: " + String(location.altitude);
    String gyro = "  g-heading: " + String(g.gyro.heading) + ", gyro.x: " + String(g.gyro.x) + ",  gyro.y: " + String(g.gyro.y) + ", gyro.z: " + String(g.gyro.z);
    String accel = "  a-heading: " + String(a.acceleration.heading) + ", accell.x: " + String(a.acceleration.x) + ",  accell.y: " + String(a.acceleration.y) + ", accell.z: " + String(a.acceleration.z);
    String t = "  Temp: " + String(temp.temperature);
    String ll = locked ? "TRUE":"FALSE";
    String status = "  locked: " + ll;
    String sig = "  signal strength [0-31]: " + signal;
    String pp = powerOn ? "ON":"OFF";
    String p = "  power enabled: " + pp;
    String b = "  battery: " + String(battery);
    Serial.println("BIKE STATE");
    Serial.println("=====================");
    Serial.println(status);
    Serial.println(p);
    Serial.println(b);
    Serial.println(sig);
    Serial.println(loc);
    Serial.println(t);
    Serial.println(gyro);
    Serial.println(accel);
    Serial.println("");
  }
};
#endif