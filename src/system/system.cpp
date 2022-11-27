#include "system.h"

namespace System
{

    void setup(void (*callback)())
    {
        pinMode(GREEN_PIN, OUTPUT);
        pinMode(BLUE_PIN, OUTPUT);
        pinMode(RED_PIN, OUTPUT);
        pinMode(ALARM_PIN, OUTPUT);
        pinMode(BLINK_PIN, OUTPUT);
        pinMode(POWER_PIN, OUTPUT);
        LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, callback, CHANGE);
    }

    void sleep()
    {
        LowPower.sleep(SLEEP_DURATION);
    }

    int setPin(int pin, bool state)
    {
        if (state)
        {
            digitalWrite(pin, HIGH);
        }
        else
        {
            digitalWrite(pin, LOW);
        }
        return 0;
    }
    int getPinState(int pin)
    {
        return digitalRead(pin);
    }
    void setGreen(bool state)
    {
        setPin(GREEN_PIN, true);
        setPin(BLUE_PIN, false);
        setPin(RED_PIN, false);
    }

    void setBlink(bool state)
    {
        setPin(BLINK_PIN, true);
    }

    void setBlue(bool state)
    {
        setPin(GREEN_PIN, false);
        setPin(BLUE_PIN, true);
        setPin(RED_PIN, false);
    }

    void setRed(bool state)
    {
        setPin(GREEN_PIN, false);
        setPin(BLUE_PIN, false);
        setPin(RED_PIN, true);
    }

    void setAlarm(bool state)
    {
        setPin(ALARM_PIN, true);
    }

    void allOff()
    {
        setPin(GREEN_PIN, false);
        setPin(BLUE_PIN, false);
        setPin(RED_PIN, false);
        setPin(ALARM_PIN, false);
        setPin(BLINK_PIN, false);
        setPin(POWER_PIN, false);
    }

    void setPower(bool state)
    {
        setPin(POWER_PIN, true);
    }

    float readBattery(bool volt, int pin)
    { // Read battery voltage
        int sensorValue = analogRead(pin);
        if (volt)
        {
            return sensorValue * (4.2 / 1023.0);
        }
        else
        {
            return sensorValue;
        }
    }
    float getBatteryVoltage()
    {
        return readBattery(true, ADC_BATTERY_MAIN);
    }

    float getReserveBatteryVoltage()
    {
        return readBattery(true, AIN_BATTERY_ALT);
    }
}