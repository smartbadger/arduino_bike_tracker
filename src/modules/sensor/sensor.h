#pragma once
#ifndef _SENSOR_H
#define _SENSOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "models.h"

#include <Wire.h>
#include "sensor.h"
#include "debugger.h"

namespace GYRO
{
    Adafruit_MPU6050 _mpu;
    boolean _ready = false;
    void sleep(boolean sleep)
    {
        _mpu.enableSleep(sleep);
    }
    void setup()
    {
        // Try to initialize!
        if (!_mpu.begin())
        {
            _ready = false;
            debuglnV("MPU6050 not Found!");
            return;
        };

        debuglnV("MPU6050 Found!");
        _ready = true;

        // setup motion detection
        _mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
        _mpu.setMotionDetectionThreshold(1);
        _mpu.setMotionDetectionDuration(20);
        _mpu.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
        _mpu.setInterruptPinPolarity(true);
        _mpu.setMotionInterrupt(true);
    }
    bool readSensor(bikedata *data)
    {
        if (!_ready)
        {
            setup();
            return false;
        }
        sensors_event_t a, g, temp;
        if (_mpu.getMotionInterruptStatus())
        {
            /* Get new sensor events with the readings */
            _mpu.getEvent(&a, &g, &temp);
            data->a = a;
            data->g = g;
            data->temp = temp.temperature;
            data->motion = true;
            debuglnV("Motion Detected");
            return true;
        }
        data->motion = false;

        return false;
    }
}
#endif