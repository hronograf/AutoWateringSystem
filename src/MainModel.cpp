#include "MainModel.h"
#include <Arduino.h>

enum Event {
    LOW_HUMIDITY,
    WATERING_COMPLETED,
    PAUSE_COMPLETED,
    BOTH_BUTTONS_PRESSED,
    BOTH_BUTTONS_HELD,
};

//  MainModel::MainModel(): _stateMachine(&_states.INITIAL){
//      // setup states
//      _states.INITIAL = State();
//      _states.WORKING = State();
//      _states.WATERING = State();
//      _states.PAUSE = State();
//      _states.SETTINGS = State();

//      // setup transitions
//      _stateMachine.add_transition(&_states.INITIAL, &_states.SETTINGS, Event::BOTH_BUTTONS_HELD);
//      _stateMachine.add_transition(&_states.INITIAL, &_states.WORKING, Event::BOTH_BUTTONS_PRESSED);

//      _stateMachine.add_transition(&_states.WORKING, &_states.SETTINGS, Event::BOTH_BUTTONS_HELD);
//      _stateMachine.add_transition(&_states.WORKING, &_states.WATERING, Event::LOW_HUMIDITY);

//      _stateMachine.add_transition(&_states.WATERING, &_states.SETTINGS, Event::BOTH_BUTTONS_HELD);   
//      _stateMachine.add_transition(&_states.WATERING, &_states.PAUSE, Event::WATERING_COMPLETED);

//      _stateMachine.add_transition(&_states.PAUSE, &_states.SETTINGS, Event::BOTH_BUTTONS_HELD);   
//      _stateMachine.add_transition(&_states.PAUSE, &_states.WORKING, Event::PAUSE_COMPLETED);
//  }

void MainModel::loopCallback() {
    buttonA.loopCallback();

    // Serial.println("MainModel::loopCallback");
    if (buttonA.isStateReady()) { // I don't need this check
        if (ButtonState::PRESSED == buttonA.getState()) {
            Serial.println("PRESSED");
        } else if (ButtonState::HELD == buttonA.getState()) {
            Serial.println("buttonA HELD!!!");
        }
    }
}
