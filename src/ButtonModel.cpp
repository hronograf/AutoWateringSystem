#include "ButtonModel.h"
#include <Arduino.h>


ButtonModel::ButtonModel(uint8_t pinNumber, uint16_t millisToBecomePressed, uint16_t millisToBecomeHeld): 
    _PIN_NUMBER(pinNumber),
    _MILLIS_TO_BECOME_PRESSED(millisToBecomePressed), 
    _MILLIS_TO_BECOME_HELD(millisToBecomeHeld),
    _lastSeenMillis(millis()),
    _highVoltageMillis(0),
    _currentState(ButtonState::INITIAL){}

void ButtonModel::loopCallback() {
    unsigned long currentMillis = millis();
    if (currentMillis < _lastSeenMillis) { // overflow check
        _lastSeenMillis = currentMillis;
    }

    if (_currentState == ButtonState::INITIAL && digitalRead(_PIN_NUMBER)) {
        _currentState = ButtonState::PROCESSING;

    } else if (_currentState == ButtonState::PROCESSING) {
        processHighVoltageMillis(currentMillis);

    } else if (_currentState == ButtonState::PRESSED) {
        _currentState = ButtonState::INITIAL;

    } else if (_currentState == ButtonState::HELD && digitalRead(_PIN_NUMBER) == 0) {
        _currentState = ButtonState::INITIAL;
    }
    _lastSeenMillis = currentMillis;
}

void ButtonModel::processHighVoltageMillis(unsigned long currentMillis) {
    if (digitalRead(_PIN_NUMBER)) {
            _highVoltageMillis += currentMillis - _lastSeenMillis;

            if (_highVoltageMillis > _MILLIS_TO_BECOME_HELD) {
                _currentState = ButtonState::HELD;
            }
        } else {
            if (_highVoltageMillis > _MILLIS_TO_BECOME_PRESSED) {
                _currentState = ButtonState::PRESSED;
            } else {
                _currentState = ButtonState::INITIAL;
            }
            _highVoltageMillis = 0;
        }
}

bool ButtonModel::isStateReady() {
    return _currentState != ButtonState::PROCESSING;
}

ButtonState ButtonModel::getState() {
    return _currentState;
}
