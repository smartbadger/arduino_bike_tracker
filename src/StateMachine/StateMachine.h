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
#include <memory>
#include <utility>
// namespace BikeState
// {
//     enum State
//     {
//         INIT,
//         LOCKED,
//         IDLE,
//         ALARM,
//         UNLOCKED,
//         READING_NFC,
//         NFC_AUTHENTICATED,
//         NFC_REJECTED,
//         SLEEP,
//         PLAYING_SOUND,
//         MAX_STATES
//     };
//     State _currentState;
//     State _previousState;
//     State _targetState;
//     Event _currentEvent;

//     void eventActionHandler(Event event);
//     void setState(int state);
//     enum State getState();
//     void update();

//     void goToSleep();
//     void goToLock();
//     void goToUnlock();
//     void goToAlarm();
//     void goToPlayingSound();
//     void goToReadingNFC();
//     void goToNFCAuthenticated();
//     void goToNFCRejected();
//     void goToIdle();

//     std::map<State, std::vector<std::pair<Event, voidFuncPtr>>> _stateEventMap = {
//         {State::INIT, {{Event::INIT_COMPLETE, goToLock}}},
//         {State::LOCKED, {{Event::MOTION_DETECTED, goToAlarm}, {Event::DETECTED_NFC, goToReadingNFC}, {Event::IS_IDLE, goToSleep}, {Event::PLAY_SOUND, goToPlayingSound}}},
//         {State::ALARM, {{Event::FINISHED_SOUND, goToLock}, {Event::DETECTED_NFC, goToReadingNFC}}},
//         {State::READING_NFC, {{Event::NFC_REJECTED, goToNFCAuthenticated}, {Event::NFC_AUTHENTICATED, goToNFCRejected}}},
//         {State::PLAYING_SOUND, {{Event::FINISHED_SOUND, goToLock}, {Event::DETECTED_NFC, goToReadingNFC}}},
//         {State::NFC_AUTHENTICATED, {{Event::LOCK, goToLock}, {Event::UNLOCK, goToUnlock}}},
//         {State::NFC_REJECTED, {{Event::LOCK, goToLock}}},
//         {State::SLEEP, {{Event::WAKEUP, goToLock}}},
//         {State::UNLOCKED, {{Event::IS_IDLE, goToLock}, {Event::DETECTED_NFC, goToReadingNFC}, {Event::PLAY_SOUND, goToPlayingSound}}}};

// };
class State {
    public:
        virtual void onEnter();
        virtual void onExit();
        virtual void update();
        virtual void eventActionHandler(Event event);
        virtual void setState(int state);
        virtual State getState();
        virtual void goToSleep();
        virtual void goToLock();
        virtual void goToUnlock();
        virtual void goToAlarm();
        virtual void goToPlayingSound();
        virtual void goToReadingNFC();
        virtual void goToNFCAuthenticated();
        virtual void goToNFCRejected();
        virtual void goToIdle();
};
};
class Error : public State
{
};
class State
{

    virtual ~State();
    virtual std::unique_ptr<State> motion_detected()
    {
        return std::unique_ptr(Error());
    };

    virtual std::unique_ptr<State> detected_nfc()
    {
        return std::unique_ptr(Error());
    };

    virtual std::unique_ptr<State> nfc_authenticated()
    {
        return std::unique_ptr(Error());
    };
};

// class Locked : public State
// {
//     unique_ptr<State> motion_detected() override
//     {
//         turnOffRed();
//         turnOnGreen();

//         return unique_ptr(Alarm());
//     }
// };

// class Alarm : public State
// {
// public:
//     Alarm()
//     {
//         // pin turn on
//     }

//     unique_ptr<State> nfc_authenticated() override
//     {
//         return unique_ptr(Unlocked());
//     }

//     ~Alarm()
//     {
//         // pin turn off
//     }

// private:
//     AlarmSound alarm_on;
// };

// class Error : public State
// {
// };

// int main()
// {
//     unique_ptr<State> state(Locked());
//     while (true)
//     {
//         state = state.detected_nfc();
//     }
// }
#endif
