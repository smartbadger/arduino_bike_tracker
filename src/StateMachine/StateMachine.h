#pragma once

#ifndef _BIKE_STATE_H
#define _BIKE_STATE_H

#include <Arduino.h>
#include "config.h"
#include "ArduinoLowPower.h"
#include <string.h>
#include <map>
#include <vector>
#include "models.h"
#include <memory>
#include <utility>
#include "debugger.h"
#include "models.h"
#include "modules/System/System.h"

class Error;

class State
{
public:
    virtual ~State();
    const BikeData &getBikeData() const; // const prevents modification of the object
    void setSensorData(sensors_event_t g, sensors_event_t a, float temp);
    void setGSM(bool network, String signal);
    void setGPS(Location location, int satellites, float speed_mph, char updated);
    void printBikeData();
    // Generic update method for all states
    virtual void update();

    // 0 is nullptr, stating it doesn't exist yet
    // virtual void update() = 0; // TODO: must be defined in child class

    // These are the transitions
    //==========================================================================
    virtual std::unique_ptr<State> motion_detected();
    virtual std::unique_ptr<State> nfc_authenticated();
    virtual std::unique_ptr<State> sleep();
    virtual std::unique_ptr<State> nfc_rejected();
    virtual std::unique_ptr<State> idle();
    virtual std::unique_ptr<State> locked();
    virtual std::unique_ptr<State> unlocked();
    virtual std::unique_ptr<State> alarm();
    // TODO: print helper
private:
    BikeData data;
};

class Error : public State
{
    // void update() override;
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
};

class Locked : public State
{
    // void update() override;

public:
    // use override to have compiler complain if you don't override the method
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
};

class Unlocked : public State
{
    // void update() override;
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
};

class Alarm : public State
{
    void update();
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
};

class Sleep : public State
{
    // void update() override;
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
    std::unique_ptr<State> idle() override;
};

class NFCAuthenticated : public State
{
    // void update() override;
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
    std::unique_ptr<State> idle() override;
};

class NFCRejected : public State
{
    // void update() override;
    std::unique_ptr<State> nfc_authenticated() override;
    std::unique_ptr<State> nfc_rejected() override;
    std::unique_ptr<State> motion_detected() override;
    std::unique_ptr<State> idle() override;
};

class NFCReading : public State
{
    // void update() override;
};

#endif