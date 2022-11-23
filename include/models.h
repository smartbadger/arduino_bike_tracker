#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <time.h>
#include <cppQueue.h>
#include "debugger.h"

#ifndef MODELS_H
#define MODELS_H

enum Event
{
    LOCK,
    UNLOCK,
    INIT_COMPLETE,
    NFC_REJECTED,
    NFC_AUTHENTICATED,
    DETECTED_NFC,
    MOTION_DETECTED,
    // IDLE,
    WAKEUP,
};

struct Location
{
    float latitude;
    float longitude;
    long altitude;
    long accuracy;
};

struct BikeData
{
    bool locked = true;
    bool motion = false;
    bool powerOn = false;
    double battery = 0.0;
    float speed_mph = 0;
    char updated;
    int satellites = 0;
    bool network = false;
    Location location = {0, 0, 0, -1};
    String signal = "0";
    sensors_event_t a;
    sensors_event_t g;
    float temp;

    // create dynamic setters and getters for each field in the struct

    void print()
    {
        String status = "";
        status += "latitude: " + String(location.latitude) + ", longitude: " + String(location.longitude) + ",  accuracy: " + String(location.accuracy) + ", altitude: " + String(location.altitude) + "\n";
        status += "satellites: " + String(satellites) + ", speed_mph: " + String(speed_mph) + ", updated: " + String(updated) + "\n";
        status += "g-heading: " + String(g.gyro.heading) + ", gyro.x: " + String(g.gyro.x) + ",  gyro.y: " + String(g.gyro.y) + ", gyro.z: " + String(g.gyro.z) + "\n";
        status += "a-heading: " + String(a.acceleration.heading) + ", accell.x: " + String(a.acceleration.x) + ",  accell.y: " + String(a.acceleration.y) + ", accell.z: " + String(a.acceleration.z) + "\n";
        status += "Temp: " + String(temp) + "\n";
        status += "locked: " + String(locked ? "TRUE" : "FALSE") + "\n";
        status += "signal strength [0-31]: " + signal + "\n";
        status += "power enabled: " + String(powerOn ? "ON" : "OFF") + "\n";
        status += "battery: " + String(battery) + "\n";
        Serial.println("BIKE STATE");
        Serial.println("=====================");
        Serial.println(status);
    }
};

// struct bikedataevents
// {
//     bikedata arr[100];
//     bikedata *pStr;
//     // cppQueue q(sizeof(String *), 3, FIFO);	// Instantiate queue
//     void example()
//     {
//         cppQueue q(sizeof(String *), 3, FIFO); // Instantiate queue
//         for (unsigned int i = 0; i < 3; i++)
//         {
//             arr[i] = arr[i];
//             pStr = &arr[i];
//             q.push(&pStr);
//         }

//         Serial.println("");
//         Serial.println("Processed text:");
//         for (unsigned int i = 0; i < 3; i++)
//         {
//             q.pop(&pStr);
//             // pStr->remove(0, 4);
//             // Serial.println(*pStr);
//         }
//     }
// };

#endif