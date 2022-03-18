// GLOBAL
#define PN532_IRQ (6)
#define PN532_RESET (7) 

#include <Wire.h>
#include <SPI.h>

#include "secrets.h"
#include "gsm_interface.h"
#include "sensor.h"
#include "sensordata.h"
#include "math.h"

// setup Serial.printlnger;

GSMInterface network = GSMInterface(20000);
Sensor sensor = Sensor();
SensorData current;

bool hasMoved(SensorData data1, SensorData data2, float threshold){
    float rd1[3] = {data1.x_rotation, data1.y_rotation, data1.z_rotation};
    float rd2[3] = {data2.x_rotation, data2.y_rotation, data2.z_rotation};
    float ad1[3] = {data1.x_acceleration, data1.y_acceleration, data1.z_acceleration};
    float ad2[3] = {data2.x_acceleration, data2.y_acceleration, data2.z_acceleration};

    float accDistance = distanceBetweenVectors(ad1, ad2);
    float rotDistance = distanceBetweenVectors(rd1, rd2);
    Serial.println("ACC & ROT");
    Serial.println(accDistance);
    Serial.println(rotDistance);
    
    if(threshold < accDistance || threshold < rotDistance){
        return true;
    }
    return false;
}

float distanceBetweenVectors(float arr1[3], float arr2[3]){
    static float deltaV[3] = {arr1[0]-arr2[0], arr1[1]-arr2[1], arr1[2]-arr2[2]};
    double magnitude = sqrt(sq(deltaV[0]) + sq(deltaV[1]) + sq(deltaV[2]));
    return (float) magnitude;
}

void setup() {

    Serial.begin(115200);
    while (!Serial)
            delay(10); // will pause Zero, Leonardo, etc until serial console opens

    // network.setup();
    sensor.setup();

}

void loop() {
   SensorData newData =  sensor.readSensor();
   if(hasMoved(newData, current, 20.0)){
       Serial.println("has moved");
   }
   current = newData;
   delay(100);
   // network.doNetworkStuff();
}