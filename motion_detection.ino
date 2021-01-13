void readGyro() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.println(sensorTemp);
  sensorTemp = getTemperature(temp.temperature);
}

String getTemperature(float temp) {
  return "Temperature: " + String(temp) + " degC";
}

void motionDetection(long elapsedTime) {
  static long sensorReadTime = 0; // interval to read nfc tag at
  int readInterval = 1000;

  sensorReadTime += elapsedTime;
  if(sensorReadTime >= readInterval){
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    readGyro();
    sensorReadTime -= readInterval;
  }
}