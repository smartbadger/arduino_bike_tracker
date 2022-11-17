#pragma once

#ifndef _System_H
#define _System_H

#include <MKRGSM.h>
#include "models.h"
#include "secrets.h"
#include "debugger.h"

class System
{

public:
    System();
    ~System();
    void sysWork();

    enum State
    {
        STARTUP,
        LOCKED,
        ALARM,
        SLEEP,
        UNLOCKED
    };
    State _currentState;
    bikedata bike;

private:
};

#endif