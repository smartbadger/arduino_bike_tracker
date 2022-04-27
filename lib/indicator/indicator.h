
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
  Indicator(int blink, int greenLED, int blueLED, int redLED, int alarm);

  void setState(int state);
  enum State getState();
  void process();

private:
  void green();
  void blue();
  void red();
  void blink();
  void off();
};

#endif