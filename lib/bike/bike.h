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
    

private:
 
};