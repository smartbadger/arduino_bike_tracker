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
    int setPin(int pin, bool state);
    int getPinState(int pin);
    void setGreen(bool state);
    void setBlink(bool state);
    void setBlue(bool state);
    void setRed(bool state);
    void setAlarm(bool state);
    void allOff();
    void setPower(bool state);
    float readBattery(bool volt, int pin);
    float getBatteryVoltage();
    float getReserveBatteryVoltage();
}

#endif