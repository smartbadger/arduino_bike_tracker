#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <Arduino.h>
#include "config.h"
#include "ArduinoLowPower.h"

namespace System
{

    void setup(void (*callback)());
    void sleep();
    int setPin(int pin, bool status);
    int getPinState(int pin);
    void setGreen(bool status);
    void setBlink(bool status);
    void setBlue(bool status);
    void setRed(bool status);
    void setAlarm(bool status);
    void setAll(bool status);
    void setPower(bool status);
    void setLock(bool status);
    float readBattery(bool volt, int pin);
    float getBatteryVoltage();
    float getReserveBatteryVoltage();
};

#endif