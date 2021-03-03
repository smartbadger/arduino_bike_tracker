#include "secrets.h"
#include "location.h"
#include "sample.h"
//#include "nfc_reader.h"
//#include "sensor.h"
#include "coordinate.h"


class Bike {

    Sample _state;
    Location _location;
    
    bool _controls;
    enum State {
        LOCKED,
        UNLOCKED,
        ALARM,
    };

    State _currentState = LOCKED;
    State _targetState = LOCKED;
    public:
        Bike() {
           _controls = false;
        }
        bool isLocked(){
            return _currentState == LOCKED || _currentState == ALARM;
        }
        void setLock(bool status) { 
            if(status){
                _currentState = LOCKED;
                _controls = false;
            }else{
                _currentState = UNLOCKED;
                _controls = true;
            }
        }
        void readNFC(){
            
        }
        Sample getSample(){
            return _state;
        }
        void setSample(Sample s){
            _state = s;
        }
    private: 

};