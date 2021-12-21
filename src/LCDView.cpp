#include "LCDView.h"

void LCDView::setup() {
    _lcd.init();                      
    _lcd.backlight();
}

void LCDView::showOptionsMenu(SettingsOption currentOption) {
    _lcd.clear();
    switch (currentOption) {
    case SettingsOption::HUMIDITY_THRESHOLD:
        printMenu(_activeOptionStr, _inactiveOptionStr, _inactiveOptionStr);
        break;
    case SettingsOption::WATERING_DURATION:
        printMenu(_inactiveOptionStr, _activeOptionStr, _inactiveOptionStr);
        break;
    case SettingsOption::PAUSE_DURATION:
        printMenu(_inactiveOptionStr, _inactiveOptionStr, _activeOptionStr);
        break;
    default:
        break;
    }
}

void LCDView::printMenu(const char* humidityStart, const char* wateringStart, const char* pauseStart) {
    _lcd.setCursor(0, 0);
    _lcd.print(humidityStart);
    _lcd.print(_humidityThresholdStr);
    _lcd.setCursor(0, 1);
    _lcd.print(wateringStart);
    _lcd.print(_wateringDurationStr);
    _lcd.setCursor(0, 2);
    _lcd.print(pauseStart);
    _lcd.print(_pauseDurationStr);
}

void LCDView::showSelectedOptionMenu(SettingsOption selectedOption, uint32_t value) {
    _lcd.clear();
    _lcd.setCursor(0, 1);
    _lcd.print("Value: ");
    switch (selectedOption) {
    case SettingsOption::HUMIDITY_THRESHOLD:
        _lcd.print(value);
        break;
    case SettingsOption::WATERING_DURATION:
    case SettingsOption::PAUSE_DURATION:
        _lcd.print(value/1000);
        _lcd.print("s");
        break;
    
    default:
        break;
    }
}

void LCDView::clearScrean() {
    _lcd.clear();
}
