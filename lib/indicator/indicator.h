
#pragma once
#ifndef _INDICATOR_H
#define _INDICATOR_H

class Indicator
{
  int _redLED;
  int _greenLED;
  int _blueLED;
  int _alarm;
  int _blink;
  int _power;

  enum State
  {
    INIT,
    LOCKED,
    UNLOCKED,
    ALARM,
    BUSY
  };

  State _currentState;
public:
  Indicator(int blink, int greenLED, int blueLED, int redLED, int alarm, int power);

  void setState(int state);
  enum State getState();
  void process();

private:
  void powerOn();
  void powerOff();
  void green();
  void blue();
  void red();
  void blink();
  void off();
};

#endif