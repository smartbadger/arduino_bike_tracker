#include <iostream>
#include <thread>
#include <chrono>
#include "headers/debug.h"
#include "headers/bike.h"

int main() {
    Bike bike = Bike();
    SensorData pos1 = {
        0,
        0.023,
        0,
        0,
        0,
        0,
        0
    };
    SensorData pos2 = {
        0,
        0.023,
        0,
        0,
        0,
        0,
        0
    };
    while(true)
    {
        log("cycle");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        bool hasMoved = true;
        if(bike.isLocked()){

        }
        // your loop body here
    }
    return 0;
}