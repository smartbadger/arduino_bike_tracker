#pragma once
#ifndef _SENSOR_H
#define _SENSOR_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <Wire.h>
#include "sensor.h"
#include "debugger.h"
#include "StateMachine/StateMachine.h"

namespace GYRO
{
    Adafruit_MPU6050 mpu;
    boolean ready = false;
    void sleep(boolean sleep)
    {
        mpu.enableSleep(sleep);
    }
    void setup()
    {
        if (!mpu.begin())
        {
            ready = false;
            debuglnV("MPU6050 not Found!");
            return;
        };

        debuglnV("MPU6050 Found!");
        ready = true;

        // setup motion detection
        mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
        mpu.setMotionDetectionThreshold(1); // TODO: There is actually a threshold built in... Do we need the other changed function?
        mpu.setMotionDetectionDuration(20);
        mpu.setInterruptPinLatch(true); // Keep it latched.  Will turn off when reinitialized.
        mpu.setInterruptPinPolarity(true);
        mpu.setMotionInterrupt(true);
    }

    // * is a pointer to the bike data and & is a reference to the bike data
    bool readSensor(State &state)
    {
        if (!ready)
        {
            setup();
            return false;
        }
        sensors_event_t a, g, temp;
        if (mpu.getMotionInterruptStatus())
        {
            /* Get new sensor events with the readings */
            // within an expression * dereferences a pointer & takes the address of a variable
            mpu.getEvent(&a, &g, &temp);
            state.setSensorData(g, a, temp.temperature);
        }

        return false;
    };
}
#endif