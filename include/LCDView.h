#ifndef LCDView_h
#define LCDView_h

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// #include <LiquidCrystal_PCF8574.h>
#include "MainModel.h"

class LCDView {
private:
    LiquidCrystal_I2C _lcd = LiquidCrystal_I2C(0x27, 20, 4);
    const char* _activeOptionStr = "[X]";
    const char* _inactiveOptionStr = "[ ]";
    const char* _humidityThresholdStr = "Humidity";
    const char* _wateringDurationStr = "Watering";
    const char* _pauseDurationStr = "Pause";

    void printMenu(const char* humidityStart, const char* wateringStart, const char* pauseStart);

public:
    void setup();

    void showOptionsMenu(SettingsOption option);
    void showSelectedOptionMenu(SettingsOption selectedOption, uint32_t value);
    void showStatus();
    void showPause();
    void clearScrean();
};

#endif
