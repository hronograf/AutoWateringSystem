#ifndef MainModel_h
#define MainModel_h

#include "ButtonModel.h"
#include "SettingsModel.h"

// struct MainStates {
//     State INITIAL;
//     State WORKING;
//     State WATERING;
//     State PAUSE;
//     State SETTINGS;
// };

enum class MainStates {
    INITIAL,
    WORKING,
    WATERING,
    PAUSE,
    SETTINGS
};


class MainModel {

private:
    const uint16_t _MILLIS_TO_BECOME_PRESSED = 20;
    const uint16_t _MILLIS_TO_BECOME_HELD = 1000;
    const uint8_t BUTTON_A_PIN_NUMBER = 7; 
    const uint8_t BUTTON_B_PIN_NUMBER = 6; 
    MainStates _currentState = MainStates::INITIAL;

    ButtonModel _buttonA = ButtonModel(BUTTON_A_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);
    ButtonModel _buttonB = ButtonModel(BUTTON_B_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);

    SettingsModel _settingsModel = SettingsModel();

public:

    void loopCallback();

    MainStates getState();
    SettingsModel& getSettingsModel();
};

#endif
