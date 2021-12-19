#include "ButtonModel.h"
#include "Fsm.h"

struct MainStates {
    State INITIAL;
    State WORKING;
    State WATERING;
    State PAUSE;
    State SETTINGS;
};


class MainModel {

private:
    const uint16_t _MILLIS_TO_BECOME_PRESSED = 40;
    const uint16_t _MILLIS_TO_BECOME_HELD = 2000;
    // MainStates _states;
    // Fsm _stateMachine;


public:
    const uint8_t BUTTON_A_PIN_NUMBER = 7; 
    ButtonModel buttonA = ButtonModel(BUTTON_A_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);

    // MainModel();
    void loopCallback();
};