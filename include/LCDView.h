#ifndef LCDView_h
#define LCDView_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MainModel.h"

class LCDView {
private:
    LiquidCrystal_I2C _lcd = LiquidCrystal_I2C(0x27, 20, 4);
    const char* _initialStr1 = "WELCOME";
    const char* _initialStr2 = "You should";
    const char* _initialStr3 = "configure device";

    const char* _activeOptionStr = "[X]";
    const char* _inactiveOptionStr = "[ ]";
    const char* _humidityThresholdStr = "Humidity";
    const char* _wateringDurationStr = "Watering";
    const char* _pauseDurationStr = "Pause";

    const char* _curHumidityStr = "CurHum: ";
    const char* _minHumidityStr = "MinHum: ";
    const char* _tflwStr = "TFLW: ";

    const char* _wateringStr = "WATERING";
    const char* _pauseStr = "PAUSE";
    const char* _waitStr = "Please wait";

    void printOptionsMenu(const char* humidityStart, const char* wateringStart, const char* pauseStart);

public:

    void setup();
    void showInitial();
    void showOptionsMenu(SettingsOption option);
    void showSelectedOptionMenu(SettingsOption selectedOption, uint32_t value);
    void showStatus(uint32_t currentHumidity, uint32_t thresholdHumidity, unsigned long millisFromLastWatering);
    void showWatering();
    void showPause();
    void clearScrean();
    void on();
    void off();
};

#endif
