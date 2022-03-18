//     #include "sensordata.h"

//     bool aboveThresholdValue(float original, float current, float threshold)
//     {
//         return abs((original - current) / original) >= threshold;
//     }

//     bool checkForMotion(SensorData current, SensorData previous)
//     {
//             float x_rotation;
//     float y_rotation;
//     float z_rotation;
//     float x_acceleration;
//     float y_acceleration;
//     float z_acceleration;
//         float t = 0.5;
//         float cr = current.rotation.calculateAverage();
//         float pr = previous.rotation.calculateAverage();
//         float ca = current.acceleration.calculateAverage();
//         float pa = previous.acceleration.calculateAverage();
//         return aboveThresholdValue(cr, pr, t) || aboveThresholdValue(ca, pa, t);
//     }
// };