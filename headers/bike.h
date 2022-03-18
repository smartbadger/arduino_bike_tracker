#include "secrets.h"
#include "location.h"
#include "sensordata.h"
#include "debug.h"

#ifndef _BIKE_H
#define _BIKE_H
class Bike
{

    SensorData _sensorData;
    Location _location;

    double _battery;
    bool _controls;
    enum State
    {
        LOCKED,
        UNLOCKED,
        ALARM,
    };

    State _currentState = LOCKED;

public:
    Bike()
    {
        log("setting up bike");
        _sensorData = SensorData();
        _location = Location();
        _battery = 0.0;
    }
    bool isLocked()
    {
        return _currentState == LOCKED || _currentState == ALARM;
    }
    void setLock(bool status)
    {
        if (status && _currentState != ALARM)
        {
            _currentState = LOCKED;

        }
        else if(status)
        {
            _currentState = UNLOCKED;
        }
    }
    SensorData getSensorData()
    {
        return _sensorData;
    }
    Location getLocationData()
    {
        return _location;
    }
    void setSensorData(SensorData s)
    {
        _sensorData = s;
    }
    void setLocationData(Location l){
        _location = l;
    }
};
#endif