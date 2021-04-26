
#ifndef _INDICATOR_H
#define _INDICATOR_H

class Indicator
{
  int _redLED;
  int _greenLED;
  int _blueLED;
  int _alarm;

  enum State
  {
    BLUE,
    BLINKING_BLUE,
    GREEN,
    RED,
    RED_BLINKING
  };

  State _currentState = RED;
  State _targetState = RED;

public:
  Indicator(int greenLED, int blueLED, int redLED, int alarm)
  {
    _greenLED = greenLED;
    _blueLED = blueLED;
    _redLED = redLED;
    _alarm = alarm;

    pinMode(_greenLED, OUTPUT);
    pinMode(_blueLED, OUTPUT);
    pinMode(_redLED, OUTPUT);

    rejected();
  }
  void ready()
  {
    _targetState = _currentState;
  }

  void reading()
  {
    _targetState = BLUE;
  }

  void rejected()
  {
    _targetState = RED;
  }

  void accepted()
  {
    _targetState = GREEN;
  }

  void loop()
  {
    if (_targetState == GREEN)
    {
      goToGreen();
    }
    else if (_targetState == BLUE)
    {
      blue();
    }
    else
    {
      goToRed();
    }
  }

private:
  void goToGreen()
  {
    switch (_currentState)
    {
    case BLUE:
      _currentState = GREEN;
      green();
      break;

    case RED:
      _currentState = GREEN;
      green();
      break;

    case GREEN:
      green();
      break;
    }
  }

  void goToRed()
  {
    switch (_currentState)
    {
    case GREEN:
      _currentState = RED;
      red();
      break;
    case BLUE: 
      _currentState = RED;
      red();
    case RED:
      red();
      break;
    }
  }

  void blinkGreen()
  {
  }

  void green()
  {
    digitalWrite(_greenLED, HIGH);
    digitalWrite(_blueLED, LOW);
    digitalWrite(_redLED, LOW);
  }

  void blue()
  {
    digitalWrite(_greenLED, LOW);
    digitalWrite(_blueLED, HIGH);
    digitalWrite(_redLED, LOW);
  }

  void red()
  {
    digitalWrite(_greenLED, LOW);
    digitalWrite(_blueLED, LOW);
    digitalWrite(_redLED, HIGH);
  }

  void off()
  {
    digitalWrite(_greenLED, LOW);
    digitalWrite(_blueLED, LOW);
    digitalWrite(_redLED, LOW);
  }
};

#endif