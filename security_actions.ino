class BikeSecurity {
    public:
        int lockState; // bike unlocked vs armed vs tracking
        int pingInterval; // time period between pings
        int bikeState; // bike controller state on/off
        int batteryLevel;

        float xRot, yRot, zRot;
        float xAcc, yAcc, zAcc;
        String sensorTemp;

        float latMov = 0, lonMov = 0;
        float latArd = 0, lonArd = 0;
    
    void printState(): 
      switch (mpu.getAccelerometerRange()) {
        case 0:
            Serial.println("State 0: Ready");
            break;
        case 1:
            Serial.println("State 1: Locked");
            break;
        case 2:
            Serial.println("State 2: Active");
            break;
        case 3:
            Serial.println("+-16G");
            break;
        }
}