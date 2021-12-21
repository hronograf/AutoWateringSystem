#include "Controller.h"

void Controller::setup() {
    Serial.begin(9600);
    _lcdView.setup();
}
void Controller::loop() {
    unsigned long currentMillis = millis();
    millisOverflowCheck(currentMillis);

    if (currentMillis - _mainModelLastMs > _mainModelLoopPeriodMs) {
        _mainModel.loopCallback();
        _mainModelLastMs = currentMillis;
    }
    if (currentMillis - _lcdViewLastMs > _lcdViewLoopPeriodMs) {
        if (_mainModel.getState() == MainStates::INITIAL) {
            _lcdView.clearScrean();
        } else if (_mainModel.getState() == MainStates::SETTINGS && _mainModel.getSettingsModel().isCurrentOptionSelected()) {
            _lcdView.showSelectedOptionMenu(_mainModel.getSettingsModel().getCurrentOption(), _mainModel.getSelectedOptionValue());
        } else if (_mainModel.getState() == MainStates::SETTINGS) {
            _lcdView.showOptionsMenu(_mainModel.getSettingsModel().getCurrentOption());
        }
        _lcdViewLastMs = currentMillis;
    }
    // delay(500);   
}

void Controller::millisOverflowCheck(unsigned long currentMillis) {
    if (_mainModelLastMs > currentMillis) {
        _mainModelLastMs = currentMillis;
    }
    if (_lcdViewLastMs > currentMillis) {
        _lcdViewLastMs = currentMillis;
    }
}
