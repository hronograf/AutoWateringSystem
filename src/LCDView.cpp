#include "LCDView.h"

void LCDView::setup() {
    _lcd.init();
    _lcd.backlight();
}

void LCDView::showInitial() {
    _lcd.clear();
    _lcd.setCursor(6, 0);
    _lcd.print(_initialStr1);
    _lcd.setCursor(5, 1);
    _lcd.print(_initialStr2);
    _lcd.setCursor(2, 2);
    _lcd.print(_initialStr3);
}

void LCDView::showOptionsMenu(SettingsOption currentOption) {
    _lcd.clear();
    switch (currentOption) {
    case SettingsOption::HUMIDITY_THRESHOLD:
        printOptionsMenu(_activeOptionStr, _inactiveOptionStr, _inactiveOptionStr);
        break;
    case SettingsOption::WATERING_DURATION:
        printOptionsMenu(_inactiveOptionStr, _activeOptionStr, _inactiveOptionStr);
        break;
    case SettingsOption::PAUSE_DURATION:
        printOptionsMenu(_inactiveOptionStr, _inactiveOptionStr, _activeOptionStr);
        break;
    default:
        break;
    }
}

void LCDView::printOptionsMenu(const char* humidityStart, const char* wateringStart, const char* pauseStart) {
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

void LCDView::showStatus(uint32_t currentHumidity, uint32_t thresholdHumidity, unsigned long millisFromLastWatering) {
    _lcd.clear();
    _lcd.setCursor(0, 1);
    _lcd.print(_curHumidityStr);
    _lcd.print(currentHumidity);
    _lcd.setCursor(0, 2);
    _lcd.print(_minHumidityStr);
    _lcd.print(thresholdHumidity);
    _lcd.setCursor(0, 3);
    _lcd.print(_tflwStr);
    _lcd.print(millisFromLastWatering / 1000);
    _lcd.print("s");
}

void LCDView::showWatering() {
    _lcd.clear();
    _lcd.setCursor(6, 1);
    _lcd.print(_wateringStr);
}

void LCDView::showPause() {
    _lcd.clear();
    _lcd.setCursor(7, 1);
    _lcd.print(_pauseStr);
    _lcd.setCursor(4, 2);
    _lcd.print(_waitStr);
}

void LCDView::clearScrean() {
    _lcd.clear();
}

void LCDView::on() {                      
    _lcd.backlight();
    // _lcd.on();
}

void LCDView::off() {
    _lcd.noBacklight();
}
