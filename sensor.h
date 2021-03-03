#include <Adafruit_MPU6050.h>
#include "coordinate.h"
#include "sample.h"
#define sfPrint(x) Serial.println(F(x));

class Sensor
{
  Adafruit_MPU6050 _mpu;
  bool _ready;

public:
  Sensor()
  {
    _ready = false;
  }
  void setup(void)
  {
    sfPrint("MPU6050 Setup");
    // Try to initialize!
    if (!_mpu.begin())
    {
      sfPrint("Failed to find MPU6050 chip");
      return;
    }
    sfPrint("MPU6050 Found!");
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    _ready = true;
  }

  Sample readSensor(long elapsedTime)
  {
    Sample current;
    static long sensorReadTime = 0; // interval to read nfc tag at
    int readInterval = 1000;

    sensorReadTime += elapsedTime;
    if (sensorReadTime >= readInterval && _ready)
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