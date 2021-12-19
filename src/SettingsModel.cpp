#include "SettingsModel.h"

void SettingsModel::next() {
    uint8_t currentOptionIndex = getCurrentOptionIndex();
    currentOptionIndex++;
    if (currentOptionIndex >= sizeof(_OPTIONS)) {
        currentOptionIndex = 0;
    }
    _currentOption = _OPTIONS[currentOptionIndex];
}

void SettingsModel::prev() {
    uint8_t currentOptionIndex = getCurrentOptionIndex();
    if (currentOptionIndex == 0) {
        currentOptionIndex = sizeof(_OPTIONS) - 1;
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
    for (uint8_t i = 0; i < sizeof(_OPTIONS); i++) {
        if (_OPTIONS[i] == _currentOption) {
            return i;
        }
    }
    return -1;
}


uint32_t SettingsModel::getHumidityThreshold() {
    return _humidityThreshold;
}

void SettingsModel::setHumidityThreshold(uint32_t threshold) {
    if (_currentOption == SettingsOption::HUMIDITY_THRESHOLD && _isCurrentOptionSelected) {
        _humidityThreshold = threshold;
    }
}

uint32_t SettingsModel::getWateringMs() {
    return _wateringMs;
}

void SettingsModel::setWateringMs(uint32_t wateringMs) {
    if (_currentOption == SettingsOption::WATERING_DURATION && _isCurrentOptionSelected) {
        if (wateringMs >= _minWateringMs) {
            _wateringMs = wateringMs;
        }
    }
}

uint32_t SettingsModel::getPauseMs() {
    return _pauseMs;
}

void SettingsModel::setPauseMs(uint32_t pauseMs) {
    if (_currentOption == SettingsOption::PAUSE_DURATION && _isCurrentOptionSelected) {
        _pauseMs = pauseMs;
    }
}