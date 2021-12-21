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
    unsigned long currentMillis = millis();
    _buttonA.loopCallback();
    _buttonB.loopCallback();

    switch (_currentState) {
    case MainStates::INITIAL:
        handleInitialState();
        break;
    case MainStates::SETTINGS:
        handleSettingsState();
        break;
    case MainStates::WORKING:
        handleWorkingState();
        break;
    case MainStates::WATERING:
        handleWateringState(currentMillis);
        break;
    case MainStates::PAUSE:
        handlePauseState(currentMillis);
        break;
    
    default:
        break;
    }

    // if (_currentState == MainStates::INITIAL) {
    //     handleInitialState();
    // } else if (_currentState == MainStates::SETTINGS) {
    //     handleSettingsState();
    // } else if (_currentState == MainStates::WORKING) {
    //     handleWorkingState();
    // } else if (_currentState == MainStates::WATERING) {
    //     handleWateringState(currentMillis);
    // } 

    if (_currentState != MainStates::WATERING && _currentState != MainStates::PAUSE) {
        _millisFromLastWatering += currentMillis - _lastSeenLoopMillis;
    }
    _lastSeenLoopMillis = currentMillis;
}

void MainModel::handleInitialState() {
    if (_shouldIgnoreButtons && (_buttonA.getState() != ButtonState::INITIAL || _buttonB.getState() != ButtonState::INITIAL)) {
        return;
    } else {
        _shouldIgnoreButtons = false;
    }

    if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
        _currentState = MainStates::SETTINGS;
        _shouldIgnoreButtons = true;
    }
}

void MainModel::handleSettingsState() {
    if (_shouldIgnoreButtons && (_buttonA.getState() != ButtonState::INITIAL || _buttonB.getState() != ButtonState::INITIAL)) {
        return;
    } else {
        _shouldIgnoreButtons = false;
    }

    if (_settingsModel.isCurrentOptionSelected()) {
        if (isButtonsPressed()) {
            _settingsModel.exit();
            _shouldIgnoreButtons = true;
            return;
        } else if ((_buttonA.getState() == ButtonState::PRESSED || _buttonA.getState() == ButtonState::HELD) && _buttonB.getState() == ButtonState::INITIAL) {
            changeOptionValue(_settingsModel.getCurrentOption(), true);
        } else if ((_buttonB.getState() == ButtonState::PRESSED || _buttonB.getState() == ButtonState::HELD) && _buttonA.getState() == ButtonState::INITIAL) {
            changeOptionValue(_settingsModel.getCurrentOption(), false);
        }

    } else {
        if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
            _currentState = MainStates::WORKING;
            _shouldIgnoreButtons = true;
            return;
        }
        if (isButtonsPressed()) {
            _settingsModel.select();
            _shouldIgnoreButtons = true;
            return;
        } else if (_buttonA.getState() == ButtonState::PRESSED && _buttonB.getState() == ButtonState::INITIAL) {
            _settingsModel.prev();
        } else if (_buttonB.getState() == ButtonState::PRESSED && _buttonA.getState() == ButtonState::INITIAL) {
            _settingsModel.next();
        }
    }
}

void MainModel::handleWorkingState() {
    if (_shouldIgnoreButtons && (_buttonA.getState() != ButtonState::INITIAL || _buttonB.getState() != ButtonState::INITIAL)) {
        return;
    } else {
        _shouldIgnoreButtons = false;
    }

    if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
        _currentState = MainStates::SETTINGS;
        _shouldIgnoreButtons = true;
        return;
    }

    uint32_t humidityResistanceSum = 0;
    for (int i = 0; i < _numberOfReadsHumiditySensor; i++) {
        humidityResistanceSum += analogRead(_HUMIDITY_SENSOR_PIN_NUMBER);
        delay(1);
    }

    _currentHumidityResistance = humidityResistanceSum / _numberOfReadsHumiditySensor;

    if (_currentHumidityResistance > _settingsModel.getHumidityThreshold() || _buttonA.getState() == ButtonState::HELD) { // todo remove HELD
        _currentState = MainStates::WATERING;
    }
}

void MainModel::handleWateringState(unsigned long currentMillis) {
    if (_shouldIgnoreButtons && (_buttonA.getState() != ButtonState::INITIAL || _buttonB.getState() != ButtonState::INITIAL)) {
        return;
    } else {
        _shouldIgnoreButtons = false;
    }
    // run motor
    Serial.println("motor running");
    _wateringMs += currentMillis - _lastSeenLoopMillis;
    if (_wateringMs >= _settingsModel.getWateringMs()) {
        // stop motor
        _millisFromLastWatering = 0;
        _wateringMs = 0;
        _currentState = MainStates::PAUSE;
        _shouldIgnoreButtons = true;
        return;
    } else if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
        // stop motor
        _millisFromLastWatering = 0;
        _wateringMs = 0;
        _currentState = MainStates::SETTINGS;
        _shouldIgnoreButtons = true;
        return;
    }
}

void MainModel::handlePauseState(unsigned long currentMillis) {
    if (_shouldIgnoreButtons && (_buttonA.getState() != ButtonState::INITIAL || _buttonB.getState() != ButtonState::INITIAL)) {
        return;
    } else {
        _shouldIgnoreButtons = false;
    }

    _pauseMs += currentMillis - _lastSeenLoopMillis;
    if (_pauseMs >= _settingsModel.getPauseMs()) {
        _pauseMs = 0;
        _currentState = MainStates::WORKING;
    } else if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
        _currentState = MainStates::SETTINGS;
        _shouldIgnoreButtons = true;
    }
}


void MainModel::changeOptionValue(SettingsOption option, bool increase) {
    switch (option) {
    case SettingsOption::HUMIDITY_THRESHOLD:
        _settingsModel.setHumidityThreshold(_settingsModel.getHumidityThreshold() + (increase ? 1 : -1));
        break;
    case SettingsOption::WATERING_DURATION:
        _settingsModel.setWateringMs(_settingsModel.getWateringMs() + (increase ? 1 : -1) * 1000);
        break;
    case SettingsOption::PAUSE_DURATION:
        _settingsModel.setPauseMs(_settingsModel.getPauseMs() + (increase ? 1 : -1) * 1000);
        break;
    
    default:
        break;
    }
}

MainStates MainModel::getState() {
    return _currentState;
}

SettingsModel& MainModel::getSettingsModel() {
    return _settingsModel;
}

unsigned long MainModel::getMillisFromLastWatering() {
    return _millisFromLastWatering;
}

uint32_t MainModel::getCurrentHumidityResistance() {
    return _currentHumidityResistance;
}

uint32_t MainModel::getSelectedOptionValue() {
    switch (_settingsModel.getCurrentOption()) {
    case SettingsOption::HUMIDITY_THRESHOLD:
        return _settingsModel.getHumidityThreshold();
        break;
    case SettingsOption::WATERING_DURATION:
        return _settingsModel.getWateringMs();
        break;
    case SettingsOption::PAUSE_DURATION:
        return _settingsModel.getPauseMs();
        break;
    
    default:
        break;
    }
    return 0;
}

bool MainModel::isButtonsPressed() {
    bool pressed = _buttonA.getState() == ButtonState::PRESSED && _buttonB.getState() == ButtonState::PRESSED;
    bool buttonAFirstLow = _buttonA.getState() == ButtonState::PRESSED && _buttonB.getState() == ButtonState::PROCESSING;
    bool buttonBFirstLow = _buttonA.getState() == ButtonState::PROCESSING && _buttonB.getState() == ButtonState::PRESSED;
    return pressed || buttonAFirstLow || buttonBFirstLow;
}
