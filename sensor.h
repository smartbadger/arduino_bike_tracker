#ifndef _SENSOR_H
#define _SENSOR_H

#include <Adafruit_MPU6050.h>
#include "coordinate.h"
#include "sensordata.h"
class Sensor
{
  Adafruit_MPU6050 _mpu;
  bool _ready;

public:
  Sensor()
  {
    _ready = false;
  }
  void setup()
  {
    Serial.println("MPU6050 Setup");
    // Try to initialize!
    if (!_mpu.begin())
    {
      Serial.println("Failed to find MPU6050 chip");
      return;
    }
    Serial.println("MPU6050 Found!");
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    _ready = true;
  }

  SensorData readSensor()
  {
    SensorData current;
    if (_ready)
    {
      /* Get new sensor events with the readings */
      sensors_event_t a, g, temp;
      _mpu.getEvent(&a, &g, &temp);

      current.acceleration = Coord(a.acceleration.x, a.acceleration.y, a.acceleration.z);
      current.rotation = Coord(g.gyro.x, g.gyro.y, g.gyro.z);
      current.temp = temp.temperature;
    }
    return current;
  }
};

#endif