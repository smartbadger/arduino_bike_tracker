// #include "bike_state.h"

// BikeState::BikeState() {}

// void BikeState::setup()
// {
//     _currentState = INIT;
// }

// void BikeState::setState(int state)
// {
//     _currentState = (State)state;
// }

// enum BikeState::State BikeState::getState()
// {
//     return _currentState;
// }

// void BikeState::processState()
// {
//     switch (_currentState)
//     {
//     case INIT:
//         init();
//         break;
//     case LOCKED:
//         locked();
//         break;
//     case UNLOCKED:
//         unlocked();
//         break;
//     case ALARM:
//         alarm();
//         break;
//     case BUSY:
//         busy();
//         break;
//     case ERROR:
//         error();
//         break;
//     case SLEEP:
//         sleep();
//         break;
//     default:
//         debuglnE("Bike State: Unknown state");
//         break;
//     }
// }

// void BikeState::init() {}
// void BikeState::locked() {}
// void BikeState::unlocked() {}
// void BikeState::alarm() {}
// void BikeState::busy() {}
// void BikeState::error() {}
// void BikeState::sleep() {}