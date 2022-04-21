#include <Arduino.h>
#include "debug.h"
#include "indicator.h"

Indicator::Indicator(int greenLED, int blueLED, int redLED)
{
  _greenLED = greenLED;
  _blueLED = blueLED;
  _redLED = redLED;
  _currentState = State::INIT;
  
  pinMode(_greenLED, OUTPUT);
  pinMode(_blueLED, OUTPUT);
  pinMode(_redLED, OUTPUT);
  off();
  process();
}

void Indicator::setState(int state){
    switch (state) {
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
      default:
          debuglnE("Invalid State");
          break;
      }
}

void Indicator::process()
{
      switch (_currentState) {
      case State::INIT:
          blue();
          break;
      case State::LOCKED:
          red();
          break;
      case State::UNLOCKED:
          green();
          break;
      case State::ALARM:
          red();
          break;
      default:
          off();
          debuglnE("Invalid State");
          break;
      }
}


void Indicator::green()
{
  digitalWrite(_greenLED, HIGH);
  digitalWrite(_blueLED, LOW);
  digitalWrite(_redLED, LOW);
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
}

