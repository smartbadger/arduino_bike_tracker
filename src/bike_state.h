#pragma once

#ifndef _BIKE_STATE_H
#define _BIKE_STATE_H

#include <Arduino.h>
#include "config.h"
#include "ArduinoLowPower.h"
#include <string.h>
#include <map>
#include <vector>
#include "StateMachine/StateMachine.h"
#include "observer.h"

class BikeEventData : public EventData
{
};

class Bike : public StateMachine
{
public:
    Bike();
    void setup();
    // void motionDetected();
    // void cardDetected();
    // void update()

private:
    enum States
    {
        INIT,
        LOCKED,
        IDLE,
        ALARM,
        NFC_AUTHENTICATING,
        NFC_AUTHENTICATED,
        NFC_REJECTED,
        SLEEP,
        MAX_STATES
    };

    void stateINIT(const NoEventData *);

    StateAction<Bike, NoEventData, &Bike::stateINIT> init;

    virtual const StateMapRowEx *GetStateMapEx() { return NULL; }
    virtual const StateMapRow *GetStateMap()
    {
        static const StateMapRow STATE_MAP[] = {
            &init};
        // C_ASSERT((sizeof(STATE_MAP)/sizeof(StateMapRow)) == MAX_STATES);
        return &STATE_MAP[0];
    }
};
namespace BikeState
{

    enum Event
    {
        INIT_COMPLETE,
        NFC_REJECTED,
        NFC_AUTHENTICATED,
        MOTION_DETECTED,
        ALARM_COMPLETED,
        DETECTED_NFC,
        IDLE,
        WAKEUP,
    };
    // q: state machine with transitions?

    State _currentState;
    State _previousState;
    State _targetState;
    Event _currentEvent;

    void eventActionHandler(Event event);
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
    // std::map<State, std::vector<std::pair<Event, State>>> _stateEventMap = {
    //     {State::INIT, {{Event::INIT_COMPLETE, State::LOCKED}}},
    //     {State::LOCKED, {{Event::MOTION_DETECTED, State::ALARM}, {Event::DETECTED_NFC, State::READING_NFC}, {Event::IDLE, State::SLEEP}}},
    //     {State::ALARM, {{Event::ALARM_COMPLETED, State::LOCKED}, {Event::DETECTED_NFC, State::READING_NFC}}},
    //     {State::READING_NFC, {{Event::NFC_REJECTED, State::LOCKED}, {Event::NFC_AUTHENTICATED, State::UNLOCKED}}},
    //     {State::SLEEP, {{Event::WAKEUP, State::LOCKED}}},
    //     {State::UNLOCKED, {{Event::IDLE, State::LOCKED}, {Event::DETECTED_NFC, State::READING_NFC}}}};
    // ;
};

    // create observable from bikedata

#endif
