
#include <Wire.h>
#include "sensor.h"
#include "debugger.h"

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

bool Sensor::readSensor(bikedata *data) {
  if(!_ready){
    setup();
    return false;
  }
  sensors_event_t a, g, temp;
  if(_mpu.getMotionInterruptStatus()) {
    /* Get new sensor events with the readings */
    _mpu.getEvent(&a, &g, &temp);
    data->a = a;
    data->g = g;
    data->temp = temp;
    data->motion = true;
    debuglnV("Motion Detected");
    return true;
  }
  data->motion= false;

  return false;
}