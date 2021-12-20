#include "Controller.h"

void Controller::setup() {
    Serial.begin(9600);
    _lcdView.setup();
}
void Controller::loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - _mainModelLastMs > _mainModelLoopPeriodMs) {
        _mainModel.loopCallback();
        _mainModelLastMs = currentMillis;
    }
    if (currentMillis - _lcdViewLastMs > _lcdViewLoopPeriodMs) {
        if (_mainModel.getState() == MainStates::INITIAL) {
            _lcdView.clearScrean();
        } else if (_mainModel.getState() == MainStates::SETTINGS) {
            _lcdView.showOptionsMenu(_mainModel.getSettingsModel().getCurrentOption());
        }
        _lcdViewLastMs = currentMillis;
    }
    // delay(500);   
}
