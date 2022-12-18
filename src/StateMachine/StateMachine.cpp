
#include "StateMachine.h"
#include "debugger.h"
using namespace System;
// Base State Class Implementation
// ============================
State::~State(){

};
const BikeData &State::getBikeData() const
{
    return this->data;
};
void State::setGSM(bool network, String signal)
{
    this->data.network = network;
    this->data.signal = signal;
}
void State::setGPS(Location location, int satellites, float speed_mph, char updated)
{
    Location old_location = this->data.location;
    this->data.location = location;
    this->data.satellites = satellites;
    this->data.speed_mph = speed_mph;
    this->data.updated = updated;
}
void State::setSensorData(sensors_event_t g, sensors_event_t a, float temp)
{
    this->data.g = g;
    this->data.a = a;
    this->data.temp = temp;
};

void State::printBikeData()
{

    String status = "";
    status += "latitude: " + String(this->data.location.latitude) + ", longitude: " + String(this->data.location.longitude) + ",  accuracy: " + String(this->data.location.accuracy) + ", altitude: " + String(this->data.location.altitude) + "\n";
    status += "satellites: " + String(this->data.satellites) + ", speed_mph: " + String(this->data.speed_mph) + ", updated: " + String(this->data.updated) + "\n";
    status += "g-heading: " + String(this->data.g.gyro.heading) + ", gyro.x: " + String(this->data.g.gyro.x) + ",  gyro.y: " + String(this->data.g.gyro.y) + ", gyro.z: " + String(this->data.g.gyro.z) + "\n";
    status += "a-heading: " + String(this->data.a.acceleration.heading) + ", accell.x: " + String(this->data.a.acceleration.x) + ",  accell.y: " + String(this->data.a.acceleration.y) + ", accell.z: " + String(this->data.a.acceleration.z) + "\n";
    status += "Temp: " + String(this->data.temp) + "\n";
    // status += "locked: " + String(this->data.locked ? "TRUE" : "FALSE") + "\n";
    status += "signal strength [0-31]: " + data.signal + "\n";
    // status += "power enabled: " + String(this->data.powerOn ? "ON" : "OFF") + "\n";
    status += "battery: " + String(this->data.battery) + "\n";
    Serial.println("BIKE STATE");
    Serial.println("=====================");
    Serial.println(status);
}

std::unique_ptr<State> State::motion_detected()
{

    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::sleep()
{
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::nfc_authenticated()
{
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::nfc_rejected()
{
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::idle()
{
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::locked()
{
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::unlocked()
{
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> State::alarm()
{
    return std::unique_ptr<State>(new Error());
};

// Locked Class Implementation
// ============================

std::unique_ptr<State> Locked::update()
{
    // debuglnE("Locked::update");
    return std::unique_ptr<State>(new Locked());
};
std::unique_ptr<State> Locked::nfc_authenticated()
{
    debuglnE("Locked::nfc_authenticated");

    System::setPower(true);
    System::setGreen(true);
    System::setAlarm(true);

    // TODO: runs for set amount of time then goes to unlocked
    return std::unique_ptr<State>(new Unlocked());
};
std::unique_ptr<State> Locked::nfc_rejected()
{
    debuglnE("Locked::nfc_rejected");
    System::setAlarm(true);
    System::setRed(true);
    System::setBlink(true);

    // TODO: runs for set amount of time then goes back to locked
    return std::unique_ptr<State>(new Locked());
};
std::unique_ptr<State> Locked::motion_detected()
{
    debuglnE("Locked::motion_detected");
    return std::unique_ptr<State>(new Alarm(millis()));
}

// Unlocked Class Implementation
// ============================
std::unique_ptr<State> Unlocked::update()
{
    // debuglnE("Unlocked::update");
    return std::unique_ptr<State>(new Unlocked());
};
std::unique_ptr<State> Unlocked::motion_detected()
{
    debuglnE("UNLOCKED::motion_detected");
    return std::unique_ptr<State>(new Unlocked());
}
std::unique_ptr<State> Unlocked::nfc_rejected()
{
    debuglnE("Unlocked::nfc_rejected");
    return std::unique_ptr<State>(new Locked());
};
std::unique_ptr<State> Unlocked::nfc_authenticated()
{
    debuglnE("Unlocked::nfc_authenticated");
    return std::unique_ptr<State>(new Locked());
};

// Alarm Class Implementation
// ============================
Alarm::Alarm(unsigned long alarm_start) : alarm_start(alarm_start){

                                          };
std::unique_ptr<State> Alarm::motion_detected()
{
    debuglnE("Alarm::motion_detected");
    return std::unique_ptr<State>(new Alarm(millis()));
}
std::unique_ptr<State> Alarm::nfc_rejected()
{
    debuglnE("Alarm::nfc_rejected");
    return std::unique_ptr<State>(new Alarm(millis()));
};
std::unique_ptr<State> Alarm::nfc_authenticated()
{
    debuglnE("Alarm::nfc_authenticated");
    return std::unique_ptr<State>(new Unlocked());
};
std::unique_ptr<State> Alarm::update()
{

    // debuglnE("Alarm::update");
    System::setAlarm(true);
    System::setRed(true);
    System::setBlink(true);
    // don't forget to set millis for time overflow
    if (millis() - alarm_start > ALARM_DURATION)
    {
        System::setAlarm(false);
        System::setRed(false);
        System::setBlink(false);
        return std::unique_ptr<State>(new Locked());
    }
    return std::unique_ptr<State>(new Alarm(this->alarm_start));
}

// Error Class Implementation
// ============================================
std::unique_ptr<State> Error::update()
{
    // debuglnE("Error::update");
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> Error::motion_detected()
{
    debuglnE("Error::motion_detected");
    return std::unique_ptr<State>(new Error());
}
std::unique_ptr<State> Error::nfc_rejected()
{
    debuglnE("Error::nfc_rejected");
    return std::unique_ptr<State>(new Error());
};
std::unique_ptr<State> Error::nfc_authenticated()
{
    debuglnE("Error::nfc_authenticated");
    return std::unique_ptr<State>(new Error());
};

// Sleep Class Implementation
// ============================================
std::unique_ptr<State> Sleep::update()
{
    // debuglnE("Sleep::update");
    return std::unique_ptr<State>(new Sleep());
};
std::unique_ptr<State> Sleep::motion_detected()
{
    debuglnE("Sleep::motion_detected");
    return std::unique_ptr<State>(new Error());
};

// NFCAuthenticated Class Implementation
// ============================================
std::unique_ptr<State> NFCAuthenticated::update()
{
    // debuglnE("NFCAuthenticated::update");
    return std::unique_ptr<State>(new NFCAuthenticated());
};
std::unique_ptr<State> NFCAuthenticated::motion_detected()
{
    debuglnE("NFCAuthenticated::motion_detected");
    return std::unique_ptr<State>(new Error());
};

// NFCRejected Class Implementation
// ============================================
std::unique_ptr<State> NFCRejected::update()
{
    // debuglnE("NFCRejected::update");
    return std::unique_ptr<State>(new NFCRejected());
};
std::unique_ptr<State> NFCRejected::motion_detected()
{
    debuglnE("NFCRejected::motion_detected");
    return std::unique_ptr<State>(new Error());
};

// NFCReading Class Implementation
// ============================================
// std::unique_ptr<State> NFCReading::motion_detected()
// {
//     debuglnE("NFCReading::motion_detected");
// };