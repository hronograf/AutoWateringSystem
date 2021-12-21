#ifndef MainModel_h
#define MainModel_h

#include <Arduino.h>
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
    const uint8_t _BUTTON_A_PIN_NUMBER = 7; 
    const uint8_t _BUTTON_B_PIN_NUMBER = 6; 

    const uint8_t _HUMIDITY_SENSOR_PIN_NUMBER = A0;
    uint32_t _currentHumidityResistance = 0xFFFFFFFF;
    const uint8_t _numberOfReadsHumiditySensor = 10;
    unsigned long _millisFromLastWatering = 0;

    MainStates _currentState = MainStates::WORKING;
    bool _shouldIgnoreButtons = false;
    unsigned long _lastSeenLoopMillis = millis();

    ButtonModel _buttonA = ButtonModel(_BUTTON_A_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);
    ButtonModel _buttonB = ButtonModel(_BUTTON_B_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);
    bool isButtonsPressed();

    SettingsModel _settingsModel = SettingsModel();

    void initialStateHandler();
    void settingsStateHandler();
    void workingStateHandler();
    void changeOptionValue(SettingsOption option, bool increase);

public:

    void loopCallback();

    MainStates getState();
    SettingsModel& getSettingsModel();
    uint32_t getSelectedOptionValue();
    unsigned long getMillisFromLastWatering();
    uint32_t getCurrentHumidityResistance();
};

#endif
