#include "MainModel.h"
#include <Arduino.h>

void MainModel::setup() {
    pinMode(_MOTOR_PIN_NUMPER, OUTPUT);
}

void MainModel::loopCallback() {
    unsigned long currentMillis = millis();
    _buttonA.loopCallback();
    _buttonB.loopCallback();

    switch (_currentState) {
    case MainState::INITIAL:
        handleInitialState();
        break;
    case MainState::SETTINGS:
        handleSettingsState();
        break;
    case MainState::WORKING:
        handleWorkingState();
        break;
    case MainState::WATERING:
        handleWateringState(currentMillis);
        break;
    case MainState::PAUSE:
        handlePauseState(currentMillis);
        break;
    
    default:
        break;
    }

    if (_currentState != MainState::WATERING && _currentState != MainState::PAUSE) {
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
        _currentState = MainState::SETTINGS;
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
            _currentState = MainState::WORKING;
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
        _currentState = MainState::SETTINGS;
        _shouldIgnoreButtons = true;
        return;
    }

    uint32_t humidityResistanceSum = 0;
    for (int i = 0; i < _numberOfReadsHumiditySensor; i++) {
        humidityResistanceSum += analogRead(_HUMIDITY_SENSOR_PIN_NUMBER);
        delay(1);
    }

    _currentHumidity = 1000/*maxHumTHresh*/-humidityResistanceSum / _numberOfReadsHumiditySensor;

    if (_currentHumidity < _settingsModel.getHumidityThreshold()) {
        _currentState = MainState::WATERING;
    }
}

void MainModel::handleWateringState(unsigned long currentMillis) {
    if (_millisFromLastWatering < _wateringInterruptMs) {
        _noInterruptWateringCount += 1;
        if (_noInterruptWateringCount >= _maxWateringNumberWithoutInterrupt) {
            _currentState = MainState::INITIAL;
            return;
        }
    } else {
        _noInterruptWateringCount = 0;
    }

    if (_shouldIgnoreButtons && (_buttonA.getState() != ButtonState::INITIAL || _buttonB.getState() != ButtonState::INITIAL)) {
        return;
    } else {
        _shouldIgnoreButtons = false;
    }
    // run motor
    digitalWrite(_MOTOR_PIN_NUMPER, HIGH);
    _wateringMs += currentMillis - _lastSeenLoopMillis;
    if (_wateringMs >= _settingsModel.getWateringMs()) {
        // stop motor
        digitalWrite(_MOTOR_PIN_NUMPER, LOW);
        _millisFromLastWatering = 0;
        _wateringMs = 0;
        _currentState = MainState::PAUSE;
        _shouldIgnoreButtons = true;
        return;
    } else if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
        // stop motor
        digitalWrite(_MOTOR_PIN_NUMPER, LOW);
        _millisFromLastWatering = 0;
        _wateringMs = 0;
        _currentState = MainState::SETTINGS;
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
        _currentState = MainState::WORKING;
    } else if (_buttonA.getState() == ButtonState::HELD && _buttonB.getState() == ButtonState::HELD) {
        _currentState = MainState::SETTINGS;
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

MainState MainModel::getState() {
    return _currentState;
}

SettingsModel& MainModel::getSettingsModel() {
    return _settingsModel;
}

unsigned long MainModel::getMillisFromLastWatering() {
    return _millisFromLastWatering;
}

uint32_t MainModel::getCurrentHumidity() {
    return _currentHumidity;
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
