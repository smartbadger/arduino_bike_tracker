#pragma once
#ifndef BIKESTATE_H
#define BIKESTATE_H

#include <Arduino.h>
#include "debugger.h"
#include <Adafruit_Sensor.h>
#include "sensor.h"
#include "location.h"

struct bikedata {
  bool locked;
  bool motion;
  Location location;
  String signal;
  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t temp;
};
#endif