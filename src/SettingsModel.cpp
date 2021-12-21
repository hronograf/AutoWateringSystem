#include "SettingsModel.h"
#include "Arduino.h"

void SettingsModel::next() {
    uint8_t currentOptionIndex = getCurrentOptionIndex();
    currentOptionIndex++;
    uint8_t optionsSize = sizeof(_OPTIONS)/sizeof(*_OPTIONS);
    if (currentOptionIndex >= optionsSize) {
        currentOptionIndex = 0;
    }
    _currentOption = _OPTIONS[currentOptionIndex];
}

void SettingsModel::prev() {
    uint8_t currentOptionIndex = getCurrentOptionIndex();
    if (currentOptionIndex == 0) {
        uint8_t optionsSize = sizeof(_OPTIONS)/sizeof(*_OPTIONS);
        currentOptionIndex = optionsSize - 1;
    } else {
        currentOptionIndex--;
    }
    _currentOption = _OPTIONS[currentOptionIndex];
}

void SettingsModel::select() {
    _isCurrentOptionSelected = true;
}

void SettingsModel::exit() {
    _isCurrentOptionSelected = false;
}

uint8_t SettingsModel::getCurrentOptionIndex() {
    uint8_t optionsSize = sizeof(_OPTIONS)/sizeof(*_OPTIONS);
    for (uint8_t i = 0; i < optionsSize; i++) {
        if (_OPTIONS[i] == _currentOption) {
            return i;
        }
    }
    return -1;
}


SettingsOption SettingsModel::getCurrentOption() {
    return _currentOption;
}

bool SettingsModel::isCurrentOptionSelected() {
    return _isCurrentOptionSelected;
}

uint32_t SettingsModel::getHumidityThreshold() {
    return _humidityThreshold;
}

void SettingsModel::setHumidityThreshold(uint32_t threshold) {
    if (_currentOption == SettingsOption::HUMIDITY_THRESHOLD && _isCurrentOptionSelected) {
        if (threshold >= _maxHumidityTHreshold){
            _humidityThreshold = _maxHumidityTHreshold;
        } else {
            _humidityThreshold = threshold;
        }
    }
}

uint32_t SettingsModel::getWateringMs() {
    return _wateringMs;
}

void SettingsModel::setWateringMs(uint32_t wateringMs) {
    if (_currentOption == SettingsOption::WATERING_DURATION && _isCurrentOptionSelected) {
        if (wateringMs > _maxWateringMs) {
            _wateringMs = _minWateringMs;
        } else if (wateringMs < _minWateringMs) {
            _wateringMs = _maxWateringMs;
        } else {
            _wateringMs = wateringMs;
        }
    }
}

uint32_t SettingsModel::getPauseMs() {
    return _pauseMs;
}

void SettingsModel::setPauseMs(uint32_t pauseMs) {
    if (_currentOption == SettingsOption::PAUSE_DURATION && _isCurrentOptionSelected) {
        if (pauseMs > _maxPauseMs) {
            _pauseMs = _minPauseMs;
        } else if (pauseMs < _minPauseMs) {
            _pauseMs = _maxPauseMs;
        } else {
            _pauseMs = pauseMs;
        }
    }
}