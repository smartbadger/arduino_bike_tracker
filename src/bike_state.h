#pragma once

#ifndef _BIKE_STATE_H
#define _BIKE_STATE_H

#include <Arduino.h>
#include "config.h"
#include "ArduinoLowPower.h"
#include <string.h>
#include <map>
#include <vector>
#include "observer.h"
#include "models.h"

// class Bike : public StateMachine
// {
// public:
//     Bike();
//     void motionDetected();
//     void nfcAuthenticated();
//     void nfcRejected();
//     void nfcReading();
//     void WakeUp();

//     // void motionDetected();
//     // void cardDetected();
//     // void update()

// private:
//     enum States
//     {
//         INIT,
//         LOCKED,
//         IDLE,
//         ALARM,
//         NFC_AUTHENTICATING,
//         NFC_AUTHENTICATED,
//         NFC_REJECTED,
//         SLEEP,
//         MAX_STATES
//     };

//     void stateINIT(const NoEventData *);
//     void stateLOCKED(const NoEventData *);
//     void stateALARM(const NoEventData *);

//     StateAction<Bike, NoEventData, &Bike::stateINIT> init;

//     virtual const StateMapRowEx *GetStateMapEx() { return NULL; }
//     virtual const StateMapRow *GetStateMap()
//     {
//         static const StateMapRow STATE_MAP[] = {
//             &init};
//         // C_ASSERT((sizeof(STATE_MAP)/sizeof(StateMapRow)) == MAX_STATES);
//         return &STATE_MAP[0];
//     }
// };
namespace BikeState
{

    enum State
    {
        INIT,
        LOCKED,
        IDLE,
        ALARM,
        UNLOCKED,
        READING_NFC,
        NFC_AUTHENTICATED,
        NFC_REJECTED,
        SLEEP,
        PLAYING_SOUND,
        MAX_STATES
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

    std::map<State, std::vector<std::pair<Event, State>>> _stateEventMap = {
        {State::INIT, {
                          {Event::INIT_COMPLETE, State::LOCKED},

                      }},
        {State::LOCKED, {
                            {Event::MOTION_DETECTED, State::ALARM},
                            {Event::DETECTED_NFC, State::READING_NFC},
                            {Event::IS_IDLE, State::SLEEP},
                            {Event::PLAY_SOUND, State::PLAYING_SOUND},
                        }},
        {State::ALARM, {
                           {Event::FINISHED_SOUND, State::LOCKED},
                           {Event::DETECTED_NFC, State::READING_NFC},
                       }},
        {State::READING_NFC, {
                                 // TODO: fix below which doesn't account for toggle
                                 {Event::NFC_REJECTED, State::LOCKED},
                                 {Event::NFC_AUTHENTICATED, State::UNLOCKED},
                             }},
        {State::PLAYING_SOUND, {
                                   {Event::FINISHED_SOUND, State::LOCKED},
                                   {Event::DETECTED_NFC, State::READING_NFC},
                               }},
        {State::SLEEP, {
                           {Event::WAKEUP, State::LOCKED},
                       }},
        {State::UNLOCKED, {
                              {Event::IS_IDLE, State::LOCKED},
                              {Event::DETECTED_NFC, State::READING_NFC},
                              {Event::PLAY_SOUND, State::PLAYING_SOUND},
                          }}};

};

    // create observable from bikedata

#endif
