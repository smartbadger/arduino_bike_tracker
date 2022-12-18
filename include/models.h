#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <time.h>
#include <cppQueue.h>
#include "debugger.h"

#ifndef MODELS_H
#define MODELS_H

enum Event
{
    LOCK,
    UNLOCK,
    INIT_COMPLETE,
    NFC_REJECTED,
    NFC_AUTHENTICATED,
    DETECTED_NFC,
    MOTION_DETECTED,
    PLAY_SOUND,
    IS_IDLE,
    FINISHED_SOUND,
    WAKEUP,
};

struct Location
{
    float latitude;
    float longitude;
    long altitude;
    long accuracy;
};

struct BikeData
{

    double battery = 0.0;
    float speed_mph = 0;
    char updated;
    int satellites = 0;
    bool network = false;
    Location location = {0, 0, 0, -1};
    String signal = "0";
    sensors_event_t a;
    sensors_event_t g;
    float temp;

    // create dynamic setters and getters for each field in the struct

    
};

#endif