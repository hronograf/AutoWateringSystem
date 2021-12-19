#include "ButtonModel.h"
#include <Arduino.h>


enum ButtonEvent {
    HIGH_VOLTAGE,
    LOW_VOLTAGE,
    f,
    PRE,
    HOLDING,
};


// ButtonStates::ButtonStates(State initial, State processing, State pressed, State held):
//     INITIAL(initial),
//     PROCESSING(processing),
//     PRESSED(pressed),
//     HELD(held){};


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

ButtonModel::ButtonModel(uint8_t pinNumber, uint16_t millisToBecomePressed, uint16_t millisToBecomeHeld): 
    _PIN_NUMBER(pinNumber),
    _MILLIS_TO_BECOME_PRESSED(millisToBecomePressed), 
    _MILLIS_TO_BECOME_HELD(millisToBecomeHeld),
    _lastSeenMillis(millis()),
    _highVoltageMillis(0),
    _stateMachine(&_buttonStates.INITIAL) // todo maybe I should reinitialize, because initial state will be assigned later
    // _buttonStates(
    //     State([](){}, [this](){this->checkVoltage();}),
    //     State([this](){this->initializeTimeVars();}, [this](){this->calcHighVoltageMillis();}),
    //     State([](){}, [this](){this->checkVoltage();}),
    //     State([](){}, [this](){this->checkVoltage();})
    // ) 
    {
    Serial.println("Start RAM:");
    Serial.println(freeRam());
    // setup pin mode
    pinMode(_PIN_NUMBER, INPUT);    

    // setup states
    _buttonStates.INITIAL = State([](){}, [this](){this->checkVoltage();});
    _buttonStates.PROCESSING = State([this](){this->initializeTimeVars();}, [this](){this->calcHighVoltageMillis();});
    _buttonStates.PRESSED = State([](){}, [this](){this->checkVoltage();});
    _buttonStates.HELD = State([](){}, [this](){this->checkVoltage();});

    // setup transitions
    // INITIAL state
    _stateMachine.add_transition(&_buttonStates.INITIAL, &_buttonStates.PROCESSING, ButtonEvent::HIGH_VOLTAGE);

    // PROCESSING state
    _stateMachine.add_transition(&_buttonStates.PROCESSING, &_buttonStates.PRESSED, ButtonEvent::PRE);
    _stateMachine.add_transition(&_buttonStates.PROCESSING, &_buttonStates.HELD, ButtonEvent::HOLDING);
    _stateMachine.add_transition(&_buttonStates.PROCESSING, &_buttonStates.INITIAL, ButtonEvent::LOW_VOLTAGE);

    // PRESSED and HELD states
    _stateMachine.add_transition(&_buttonStates.PRESSED, &_buttonStates.INITIAL, ButtonEvent::LOW_VOLTAGE);
    _stateMachine.add_transition(&_buttonStates.HELD, &_buttonStates.INITIAL, ButtonEvent::LOW_VOLTAGE);
    Serial.println("End RAM:");
    Serial.println(freeRam());
}

void ButtonModel::checkVoltage() {
    // Serial.println("ButtonModel::checkVoltage");
    if (digitalRead(_PIN_NUMBER)) {
        _stateMachine.trigger(ButtonEvent::HIGH_VOLTAGE);
    } else {
        _stateMachine.trigger(ButtonEvent::LOW_VOLTAGE);
    }
}

void ButtonModel::initializeTimeVars() {
    _lastSeenMillis = millis();
    _highVoltageMillis = 0;
}

void ButtonModel::calcHighVoltageMillis() {
    Serial.println("calcHighVoltage RAM:");
    Serial.println(freeRam());
    unsigned long currentMillis = millis();
    if (currentMillis < _lastSeenMillis) { // overflow check
        _lastSeenMillis = currentMillis;
    }

    if (digitalRead(_PIN_NUMBER)) {
        _highVoltageMillis += currentMillis - _lastSeenMillis;

        if (_highVoltageMillis > _MILLIS_TO_BECOME_HELD) {
            _stateMachine.trigger(ButtonEvent::HOLDING);
            // Serial.println("hold");
        }

    } else {
        Serial.println("bug");
        Serial.println(freeRam());
        if (_highVoltageMillis > _MILLIS_TO_BECOME_PRESSED) {
            // Serial.println("pred");
            _stateMachine.trigger(ButtonEvent::PRE);
            // Serial.println("pre end");
            
        } else {
            _stateMachine.trigger(ButtonEvent::LOW_VOLTAGE);
            // Serial.println("LOW_VOLTAGE");
        }
    }

    _lastSeenMillis = currentMillis;
}


bool ButtonModel::isStateReady() {
    return &_stateMachine.get_state() != &_buttonStates.PROCESSING;
}

const State& ButtonModel::getState() {
    return _stateMachine.get_state();
}

void ButtonModel::loopCallback() {
    _stateMachine.run_machine();
}

const ButtonStates& ButtonModel::getButtonStates() {
    return _buttonStates;
}


// void ButtonModel::loopCallback(int milliseconds) { // maybe _current_millis = millis()?
//     if (digitalRead(_PIN_NUMBER)) { // if button pressed
//         switch (_savedState) {
//             case ButtonState::CHECKED:
//                 _savedState = ButtonState::PROCESSING;
//                 break;
//             case ButtonState::PROCESSING:
//                 _highVoltageMillis += milliseconds - _lastSeenMillis; // if milliseconds < _lastSeenMillis: _lastSeenMillis = milliseconds
//                 break;
//             default:
//                 break;
//         }

//     } else {
//         if (_savedState == ButtonState::PROCESSING) {
//             if (_highVoltageMillis >= _MILLIS_TO_BECOME_HELD) {
//                 _savedState = ButtonState::HELD;
//             } else if (_highVoltageMillis >= _MILLIS_TO_BECOME_PRESSED) {
//                 _savedState = ButtonState::PRESSED;
//             } else {
//                 _savedState = ButtonState::CHECKED;
//             }
//         }
//         _highVoltageMillis = 0;
//     }
//     _lastSeenMillis = milliseconds;
// }
