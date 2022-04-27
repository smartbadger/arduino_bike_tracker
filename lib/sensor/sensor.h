#pragma once 
#ifndef _SENSOR_H
#define _SENSOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "bikestate.h"

class Sensor
{
    Adafruit_MPU6050 _mpu;
    bool _ready;
    public:
        Sensor();
        void setup();
        // bool readSensor(sensors_event_t *a, sensors_event_t *g, sensors_event_t *temp);
        bool readSensor(bikedata *data);
};

#endif