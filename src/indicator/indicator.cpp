#include "indicator.h"

Indicator::Indicator(int blink, int greenLED, int blueLED, int redLED, int alarm, int power)
{
    _greenLED = greenLED;
    _blueLED = blueLED;
    _redLED = redLED;
    _alarm = alarm;
    _blink = blink;
    _power = power;
    _currentState = State::INIT;

    pinMode(_greenLED, OUTPUT);
    pinMode(_blueLED, OUTPUT);
    pinMode(_redLED, OUTPUT);
    pinMode(_alarm, OUTPUT);
    pinMode(_blink, OUTPUT);
    pinMode(_power, OUTPUT);
}

void Indicator::setState(int state)
{
    switch (state)
    {
    case 0:
        _currentState = State::INIT;
        break;
    case 1:
        _currentState = State::LOCKED;
        break;
    case 2:
        _currentState = State::UNLOCKED;
        break;
    case 3:
        _currentState = State::ALARM;
        break;
    case 4:
        _currentState = State::BUSY;
        break;
    default:
        debuglnE("Invalid State");
        break;
    }
}

void Indicator::process()
{
    switch (_currentState)
    {
    case State::INIT:
        off();
        powerOff();
        blue();
        break;
    case State::LOCKED:
        off();
        powerOff();
        red();
        break;
    case State::UNLOCKED:
        off();
        green();
        powerOn();
        break;
    case State::ALARM:
        off();
        powerOff();
        blink();
        red();
        break;
    case State::BUSY:
        off();
        blink();
        blue();
        break;
    default:
        off();
        powerOff();
        debuglnE("Invalid State");
        break;
    }
}

Indicator::State Indicator::getState()
{
    return _currentState;
}
void Indicator::sleep()
{
    off();
}
void Indicator::setup()
{
    debuglnV("Setting up Indicator...");
    process();
}
void Indicator::green()
{
    digitalWrite(_greenLED, HIGH);
    digitalWrite(_blueLED, LOW);
    digitalWrite(_redLED, LOW);
}

void Indicator::blink()
{
    digitalWrite(_blink, HIGH);
}

void Indicator::blue()
{
    digitalWrite(_greenLED, LOW);
    digitalWrite(_blueLED, HIGH);
    digitalWrite(_redLED, LOW);
}

void Indicator::red()
{
    digitalWrite(_greenLED, LOW);
    digitalWrite(_blueLED, LOW);
    digitalWrite(_redLED, HIGH);
}

void Indicator::off()
{
    digitalWrite(_greenLED, LOW);
    digitalWrite(_blueLED, LOW);
    digitalWrite(_redLED, LOW);
    digitalWrite(_alarm, LOW);
    digitalWrite(_blink, LOW);
}

void Indicator::powerOn()
{
    digitalWrite(_power, HIGH);
}

void Indicator::powerOff()
{
    digitalWrite(_power, LOW);
}