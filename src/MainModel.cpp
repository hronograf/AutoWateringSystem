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
    _buttonA.loopCallback();
    _buttonB.loopCallback();

    if (_currentState == MainStates::INITIAL) {
        if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
            _currentState = MainStates::SETTINGS;
            Serial.println("to SETTINGS");
        }
    } else if (_currentState == MainStates::SETTINGS) {
        // Serial.println("buttonaB: ");
        // if (_buttonB.getState() == ButtonState::INITIAL)
        //     Serial.println("INITIAL");
        // if (_buttonB.getState() == ButtonState::PROCESSING)
        //     Serial.println("PROCESSING");
        // if (_buttonB.getState() == ButtonState::PRESSED)
        //     Serial.println("PRESSED");
        // if (_buttonB.getState() == ButtonState::HELD)
        //     Serial.println("HELD");
        // if (_settingsModel.isCurrentOptionSelected()) {
            
        // }
        if (_buttonA.getState() == ButtonState::PRESSED && _buttonB.getState() == ButtonState::PRESSED) {
            _currentState = MainStates::INITIAL;
        } else if (_buttonA.getState() == ButtonState::PRESSED) {
            _settingsModel.prev();
        } else if (_buttonB.getState() == ButtonState::PRESSED) {
            _settingsModel.next();
        }
    }
    

    // Serial.println("MainModel::loopCallback");
    // if (buttonA.isStateReady()) { // I don't need this check
    //     if (ButtonState::PRESSED == buttonA.getState()) {
    //         Serial.println("PRESSED");
    //     } else if (ButtonState::HELD == buttonA.getState()) {
    //         Serial.println("buttonA HELD!!!");
    //     }
    // }
}

MainStates MainModel::getState() {
    return _currentState;
}

SettingsModel& MainModel::getSettingsModel() {
    return _settingsModel;
}
