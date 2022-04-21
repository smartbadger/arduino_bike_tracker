
#pragma once
#ifndef _INDICATOR_H
#define _INDICATOR_H

class Indicator
{
  int _redLED;
  int _greenLED;
  int _blueLED;
  
  enum State
  {
    INIT,
    LOCKED,
    UNLOCKED,
    ALARM
  };

  State _currentState;
public:
  Indicator(int greenLED, int blueLED, int redLED);

  void setState(int state);

  void process();

private:
  void green();
  void blue();
  void red();
  void off();
};

#endif