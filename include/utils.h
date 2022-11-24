#pragma once
#include "models.h"
#include "math.h"

#ifndef UTILS_H
#define UTILS_H

// q: how do i fix multiple definition error for this function?
// a: https://stackoverflow.com/questions/105252/how-do-i-compile-a-c-program-with-multiple-source-files

static bool sensorHasChangedWithThresholds(sensors_event_t sensor, sensors_event_t old_sensor, float threshold)
{
    return (abs(sensor.acceleration.x - old_sensor.acceleration.x) > threshold) ||
           (abs(sensor.acceleration.y - old_sensor.acceleration.y) > threshold) ||
           (abs(sensor.acceleration.z - old_sensor.acceleration.z) > threshold) ||
           (abs(sensor.gyro.x - old_sensor.gyro.x) > threshold) ||
           (abs(sensor.gyro.y - old_sensor.gyro.y) > threshold) ||
           (abs(sensor.gyro.z - old_sensor.gyro.z) > threshold);
};
static bool locationHasChangedWithThresholds(Location a, Location b, float threshold)
{
    return (abs(a.latitude - b.latitude) < threshold && abs(a.longitude - b.longitude) < threshold);
};

#endif