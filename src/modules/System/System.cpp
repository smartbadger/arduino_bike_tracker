#include "System.h"

void System::setup(void (*callback)())
{
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(ALARM_PIN, OUTPUT);
    pinMode(BLINK_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, callback, CHANGE);
}

void System::sleep()
{
    LowPower.sleep(SLEEP_DURATION);
}

int System::setPin(int pin, bool status)
{
    if (status)
    {
        digitalWrite(pin, HIGH);
    }
    else
    {
        digitalWrite(pin, LOW);
    }
    return 0;
}
int System::getPinState(int pin)
{
    return digitalRead(pin);
}
void System::setGreen(bool status)
{
    setPin(GREEN_PIN, status);
    setPin(BLUE_PIN, !status);
    setPin(RED_PIN, !status);
}

void System::setBlink(bool status)
{
    setPin(BLINK_PIN, status);
}

void System::setBlue(bool status)
{
    setPin(GREEN_PIN, !status);
    setPin(BLUE_PIN, status);
    setPin(RED_PIN, !status);
}

void System::setRed(bool status)
{
    setPin(GREEN_PIN, !status);
    setPin(BLUE_PIN, !status);
    setPin(RED_PIN, status);
}

void System::setAlarm(bool status)
{
    setPin(ALARM_PIN, status);
}

void System::setAll(bool status)
{
    setPin(GREEN_PIN, !status);
    setPin(BLUE_PIN, !status);
    setPin(RED_PIN, !status);
    setPin(ALARM_PIN, !status);
    setPin(BLINK_PIN, !status);
    setPin(POWER_PIN, !status);
}

// TODO: potential future feature where the lock is controlled by the system
void System::setLock(bool status)
{
}

void System::setPower(bool status)
{
    setPin(POWER_PIN, status);
}

float System::readBattery(bool volt, int pin)
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
float System::getBatteryVoltage()
{
    return readBattery(true, ADC_BATTERY_MAIN);
}

float System::getReserveBatteryVoltage()
{
    return readBattery(true, AIN_BATTERY_ALT);
}
