#ifndef MainModel_h
#define MainModel_h

#include <Arduino.h>
#include "ButtonModel.h"
#include "SettingsModel.h"

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
    const uint8_t _MOTOR_PIN_NUMPER = 12;

    const uint8_t _HUMIDITY_SENSOR_PIN_NUMBER = A0;
    uint32_t _currentHumidityResistance = 0xFFFFFFFF;
    const uint8_t _numberOfReadsHumiditySensor = 10;
    unsigned long _millisFromLastWatering = 0;
    uint32_t _wateringMs = 0;
    uint32_t _pauseMs = 0;

    MainStates _currentState = MainStates::INITIAL;
    bool _shouldIgnoreButtons = false;
    unsigned long _lastSeenLoopMillis = millis();

    ButtonModel _buttonA = ButtonModel(_BUTTON_A_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);
    ButtonModel _buttonB = ButtonModel(_BUTTON_B_PIN_NUMBER, _MILLIS_TO_BECOME_PRESSED, _MILLIS_TO_BECOME_HELD);
    bool isButtonsPressed();

    SettingsModel _settingsModel = SettingsModel();

    void handleInitialState();
    void handleSettingsState();
    void handleWorkingState();
    void handleWateringState(unsigned long currentMillis);
    void handlePauseState(unsigned long currentMillis);

    void changeOptionValue(SettingsOption option, bool increase);

public:

    void setup();

    void loopCallback();

    MainStates getState();
    SettingsModel& getSettingsModel();
    uint32_t getSelectedOptionValue();
    unsigned long getMillisFromLastWatering();
    uint32_t getCurrentHumidityResistance();
};

#endif
