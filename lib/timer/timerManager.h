/*
	timerManager.h - A singleton that holds all the Timer instances. It's used to
  update all timers at once

  Link - https://github.com/brunocalou/Timer

	Created by Bruno Calou Alves, Apr, 2017.
	Read LICENSE for more information.
*/

#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H


#include "Arduino.h"


#ifndef NULL
#define NULL 0
#endif

#include "timer.h"

class TimerManager {
  friend class Timer;

public:
  static TimerManager& instance();

  /**
     Updates all timers
   */
  void update();

  /*
		Starts all timers
	*/
	void start();

	/*
		Stops and resets all timers
	*/
	void stop();

	/*
		Pauses all timers
	*/
	void pause();

	/*
		Resets elapsed time of all timers. If any timer was running, it will continue to do so
	*/
	void reset();

private:
  TimerManager();
  TimerManager(TimerManager const &);
  void operator=(TimerManager const &);

  /**
     Adds a timer to the TimerManager
   */
  void add(Timer *timer);

  /**
     Removes a timer from the TimerManager
     @return True if the timer was removed, false otherwise
   */
  bool remove(Timer *timer);

  class TimerNode {
  public:
    TimerNode(Timer *timer) {
      next = NULL;
      value = timer;
    }
    TimerNode *next;
    Timer *value;
  };

  TimerNode *first;
  TimerNode *last;
};
#endif //TIMERMANAGER_H
