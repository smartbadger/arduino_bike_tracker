#include "secrets.h"
#include "location.h"
#include "sensordata.h"


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

    }
    bool isLocked()
    {
        return _currentState == LOCKED || _currentState == ALARM;
    }
    void setLock(bool status)
    {
        if (status)
        {
            _currentState = LOCKED;

        }
        else
        {
            _currentState = UNLOCKED;
        }
    }
    SensorData getSensorData()
    {
        return _sensorData;
    }
    void setSensorData(SensorData s)
    {
        _sensorData = s;
    }
};