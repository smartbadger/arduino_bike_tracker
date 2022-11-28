#pragma once

#ifndef _OBSERVER_H
#define _OBSERVER_H
#include <Adafruit_Sensor.h>
#include "models.h"
#include <string.h>
#include <map>
#include <vector>
#include "utils.h"

template <typename T>
struct Observer
{
    virtual void handleEvent(enum Event event) = 0;
    // virtual void field_changed(T &source, const String &field_name) = 0;
};
template <typename T>
struct Observable
{
    // void notify(T &source, const String &field_name)
    // {
    //     for (auto observer : m_observers)
    //         observer->field_changed(source, field_name);
    // }
    void notify(enum Event event)
    {
        for (auto observer : m_observers)
            observer->handleEvent(event);
    }
    void subscribe(Observer<T> &observer) { m_observers.push_back(&observer); }
    void unsubscribe(Observer<T> &observer)
    {
        m_observers.erase(remove(m_observers.begin(), m_observers.end(), &observer), m_observers.end());
    }

private:
    std::vector<Observer<T> *> m_observers;
};

// create observable from bikedata
struct BikeDataObservable : public Observable<BikeDataObservable>
{
    BikeDataObservable(BikeData &bikeData) : m_bikeData(bikeData) {}
    BikeDataObservable() : m_bikeData(BikeData()) {}

    // create dynamic setters and getters for each field in the struct

    void print()
    {
        this->m_bikeData.print();
    }
    void toggleLocked()
    {

        this->setLocked(!this->m_bikeData.locked);
    }
    void setLocked(bool locked)
    {
        bool old_locked = this->m_bikeData.locked;
        this->m_bikeData.locked = locked;
        if (old_locked != locked)
        {
            locked ? notify(LOCK) : notify(UNLOCK);
        }
    }
    void setGyro(sensors_event_t g, sensors_event_t a, float temp)
    {
        sensors_event_t old_g = this->m_bikeData.g;
        sensors_event_t old_a = this->m_bikeData.a;
        this->m_bikeData.g = g;
        this->m_bikeData.a = a;
        this->m_bikeData.temp = temp;

        if (sensorHasChangedWithThresholds(old_a, a, 1) || sensorHasChangedWithThresholds(old_g, g, 1))
            notify(MOTION_DETECTED);
    }

    void setGPS(Location location, int satellites, float speed_mph, char updated)
    {
        Location old_location = this->m_bikeData.location;
        this->m_bikeData.location = location;
        this->m_bikeData.satellites = satellites;
        this->m_bikeData.speed_mph = speed_mph;
        this->m_bikeData.updated = updated;
        if (locationHasChangedWithThresholds(old_location, location, 1))
            notify(MOTION_DETECTED);
    }
    void setGSM(bool network, String signal)
    {
        this->m_bikeData.network = network;
        this->m_bikeData.signal = signal;
    }
    BikeData getBikeData() { return m_bikeData; }

private:
    BikeData m_bikeData;
};

struct EventObserver : Observer<BikeDataObservable>
{
    void handleEvent(enum Event event)
    {

        switch (event)
        {
        case LOCK:
            Serial.println("LOCKED");
            break;
        case UNLOCK:
            Serial.println("UNLOCKED");
            break;
        case INIT_COMPLETE:
            Serial.println("INIT_COMPLETE");
            break;
        case NFC_REJECTED:
            Serial.println("NFC_REJECTED");
            break;
        case NFC_AUTHENTICATED:
            Serial.println("NFC_AUTHENTICATED");
            break;
        case DETECTED_NFC:
            Serial.println("DETECTED_NFC");
            break;
        case MOTION_DETECTED:
            Serial.println("MOTION_DETECTED");
            break;
        case WAKEUP:
            Serial.println("WAKEUP");
            break;
        // case IDLE:
        //     Serial.println("IDLE");
        //     break;
        default:
            break;
        }
    }
};

#endif