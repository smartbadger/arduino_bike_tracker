#pragma once

#ifndef _BIKE_STATE_H
#define _BIKE_STATE_H

#include <Arduino.h>
#include "config.h"
#include "ArduinoLowPower.h"
#include <string.h>
#include <map>
#include <vector>

namespace BikeState
{
    enum State
    {
        INIT,
        LOCKED,
        UNLOCKED,
        ALARM,
        BUSY,
        SLEEP
    };

    enum Event
    {
        INIT_EVENT,
        NFC_READ,
        MOTION_DETECTED,
        TIMEOUT,
        GSM_TRANSMIT,
        IDLE,
        WAKEUP
    };

    State _currentState;
    State _targetState;

    std::map<State, std::vector<std::pair<Event, State>>> _stateEventMap = {
        {State::INIT, {{Event::INIT_EVENT, State::LOCKED}}},
        {State::LOCKED, {{Event::NFC_READ, State::UNLOCKED}, {MOTION_DETECTED, State::ALARM}}},
        {State::UNLOCKED, {{Event::IDLE, State::LOCKED}, {Event::NFC_READ, State::LOCKED}}},
        {State::ALARM, {{Event::TIMEOUT, State::LOCKED}}},
        {State::BUSY, {{Event::GSM_TRANSMIT, State::LOCKED}}},
        {State::SLEEP, {{Event::WAKEUP, State::INIT}}}};
    ;
    void setState(int state);
    enum State getState();
    void processState();
    void setup();

    void init();
    void locked();
    void unlocked();
    void alarm();
    void busy();
    void error();
    void sleep();
};

#endif
