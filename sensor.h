#ifndef SENSOR_H
#define SENSOR_H

#include <Adafruit_MPU6050.h>
#include "coordinate.h"
#include "sensordata.h"

class Sensor
{
  Adafruit_MPU6050 _mpu;
  bool _ready;

  public:
  	Sensor();
    ~Sensor();
    void setup();
    SensorData readSensor();
};

#endif