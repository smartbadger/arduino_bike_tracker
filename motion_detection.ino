bool checkForMotion() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float thresholdLevel = .5; // percentage increase or decrease
  
  float lastAccAvg = calculateAverage(xAcc, yAcc, zAcc);
  float curAccAvg = calculateAverage(a.acceleration.x, a.acceleration.y, a.acceleration.z);

  float lastRotAvg = calculateAverage(xRot, yRot, zRot);
  float curRotAvg = calculateAverage(g.gyro.x, g.gyro.y, g.gyro.z);

  // Set new datapoints
  xRot = g.gyro.x;
  yRot = g.gyro.y;
  zRot = g.gyro.z;
 
  xAcc = a.acceleration.x;
  yAcc = a.acceleration.y;
  zAcc = a.acceleration.z;

  sensorTemp = getTemperature(temp.temperature);

  if( aboveThresholdValue(lastAccAvg, curAccAvg, thresholdLevel)|| aboveThresholdValue(lastRotAvg, curRotAvg, thresholdLevel)){
    return true;
  }
  return false;
}

String getTemperature(float temp) {
  return "Temperature: " + String(temp) + " degC";
}

float calculateAverage(float x, float y, float z){
  return (x+y+z)/3;
}

bool aboveThresholdValue (float original, float current, float threshold){
  return abs((original - current)/original) >= threshold;
}

bool motionDetection(long elapsedTime) {
  static long sensorReadTime = 0; // interval to read nfc tag at
  int readInterval = 1000;

  sensorReadTime += elapsedTime;
  if(sensorReadTime >= readInterval){
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    sensorReadTime -= readInterval;
    return checkForMotion();
  }
  return false;
}