#include "secrets.h"
#include "location.h"
#include "sensordata.h"

#include "sensor.h"
#include "coordinate.h"
#include "indicator.h"

class Bike
{

    SensorData _state;
    Location _location;
    Sensor _sen;

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
    Bike(Sensor sensor)
    {
        _sen = sensor;
        _controls = false;
    }
    void setup()
    {
        _sen.setup();
    }
    void loop() {
        
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
            _controls = false;
        }
        else
        {
            _currentState = UNLOCKED;
            _controls = true;
        }
    }
    SensorData getSensorData()
    {
        return _state;
    }
    void setSensorData(SensorData s)
    {
        _state = s;
    }
    bool checkForMotion()
    {
        SensorData s = _sen.readSensor();
        bool motion = checkForMotion(getSensorData(), s);
        setSensorData(s);
        return motion;
    }

private:
    bool aboveThresholdValue(float original, float current, float threshold)
    {
        return abs((original - current) / original) >= threshold;
    }

    bool checkForMotion(SensorData current, SensorData previous)
    {
        float t = 0.5;
        float cr = current.rotation.calculateAverage();
        float pr = previous.rotation.calculateAverage();
        float ca = current.acceleration.calculateAverage();
        float pa = previous.acceleration.calculateAverage();
        return aboveThresholdValue(cr, pr, t) || aboveThresholdValue(ca, pa, t);
    }
};