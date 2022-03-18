#include "sensor.h"
#include "sensordata.h"


Sensor::Sensor(){
  Adafruit_MPU6050 _mpu;
  bool _ready = false;
}

Sensor::~Sensor(){
  // destroy?
}

void Sensor::setup() {
    Serial.println("MPU6050 starting setup");
    // Try to initialize!
    _ready = _mpu.begin();
    if (!_ready)
    {
      Serial.println("Failed to find MPU6050 chip");
      return;
    }
    Serial.println("MPU6050 Found!");
    _mpu.enableCycle(true);
    _mpu.setCycleRate(MPU6050_CYCLE_5_HZ);
    _mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

}

SensorData Sensor::readSensor()
  {
    SensorData current;
    if (_ready)
    {
      /* Get new sensor events with the readings */
      sensors_event_t a, g, temp;
      _mpu.getEvent(&a, &g, &temp);

     
      current.x_acceleration = a.acceleration.x;
      current.y_acceleration = a.acceleration.y;
      current.z_acceleration = a.acceleration.z;
      current.x_rotation = a.gyro.x;
      current.y_rotation = a.gyro.y;
      current.z_rotation = a.gyro.z;
      current.temp = temp.temperature;
    }
    return current;
  }

 