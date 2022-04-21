
#include <Wire.h>
#include "sensor.h"
#include "debug.h"

Adafruit_MPU6050 _mpu;

Sensor::Sensor(){
  _mpu = _mpu;
  _ready = false;
};

void Sensor::setup() {
  // Try to initialize!
  if (!_mpu.begin()) {
    _ready = false;
    debuglnV("MPU6050 not Found!");
    return;
  };

  debuglnV("MPU6050 Found!");
  _ready = true;

  //setup motion detection
  _mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  _mpu.setMotionDetectionThreshold(1);
  _mpu.setMotionDetectionDuration(20);
  _mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  _mpu.setInterruptPinPolarity(true);
  _mpu.setMotionInterrupt(true);

}

bool Sensor::readSensor(sensors_event_t a, sensors_event_t g, sensors_event_t temp) {
  if(!_ready){
    setup();
    return false;
  }
  if(_mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    _mpu.getEvent(&a, &g, &temp);
    debuglnV("Motion Detected");
    /* Print out the values */
    // Serial.print("AccelX:");
    // Serial.print(a.acceleration.x);
    // Serial.print(",");
    // Serial.print("AccelY:");
    // Serial.print(a.acceleration.y);
    // Serial.print(",");
    // Serial.print("AccelZ:");
    // Serial.print(a.acceleration.z);
    // Serial.print(", ");
    // Serial.print("GyroX:");
    // Serial.print(g.gyro.x);
    // Serial.print(",");
    // Serial.print("GyroY:");
    // Serial.print(g.gyro.y);
    // Serial.print(",");
    // Serial.print("GyroZ:");
    // Serial.print(g.gyro.z);
    debuglnV("");
    return true;
  }

  return false;
}