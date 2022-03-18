#include "sensordata.h"
#include "math.h"

#ifndef HELPERS_H
#define HELPERS_H

bool hasMoved(SensorData data1, SensorData data2, float threshold){
    float rd1[3] = {data1.x_rotation, data1.y_rotation, data1.z_rotation};
    float rd2[3] = {data2.x_rotation, data2.y_rotation, data2.z_rotation};
    float ad1[3] = {data1.x_acceleration, data1.y_acceleration, data1.z_acceleration};
    float ad2[3] = {data2.x_acceleration, data2.y_acceleration, data2.z_acceleration};

    float accDistance = distanceBetweenVectors(ad1, ad2);
    float rotDistance = distanceBetweenVectors(rd1, rd2);
    
    if(threshold < accDistance || threshold < rotDistance){
        return true;
    }
    return false;
}
float sq(float num){
    return num * num;
}
float distanceBetweenVectors(float arr1[3], float arr2[3]){
    static float deltaV[3] = {arr1[0]-arr2[0], arr1[1]-arr2[1], arr1[2]-arr2[2]};
    double magnitude = sqrt(sq(deltaV[0]) + sq(deltaV[1]) + sq(deltaV[2]));
    return (float) magnitude;
}

#endif