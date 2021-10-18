#include "coordinate.h"

#ifndef _SensorData_H
#define _SensorData_H

struct SensorData {
    float temp;
    Coord rotation;
    Coord acceleration;
};
#endif