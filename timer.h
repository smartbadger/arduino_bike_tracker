#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{

    unsigned long time_now = 0;
    long timerStart = 0;
    long timerTarget = 0;

public:
    void startTimer(int msec)
    {
        timerStart = millis();
        timerTarget = msec;
    }

    bool isTimerReady()
    {
        return (millis() - timerStart) > timerTarget;
    }

    void loop(int period)
    {
        if ((unsigned long)(millis() - time_now) >= period)
        {
            time_now = millis();
            Serial.println("Hello");
        }

        //Run other code
    }
};

#endif // _TIMER_H_
