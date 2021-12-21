#include "Controller.h"

void Controller::setup() {
    Serial.begin(9600);
    _mainModel.setup();
    _lcdView.setup();
}
void Controller::loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - _mainModelLastMs > _mainModelLoopPeriodMs) {
        _mainModel.loopCallback();
        _mainModelLastMs = currentMillis;
    }

    if (currentMillis - _lcdViewLastMs > _lcdViewLoopPeriodMs) {
        switch (_mainModel.getState()) {
        case MainStates::INITIAL:
            _lcdView.showInitial();
            break;
        case MainStates::SETTINGS:
            if (_mainModel.getSettingsModel().isCurrentOptionSelected()) {
                _lcdView.showSelectedOptionMenu(_mainModel.getSettingsModel().getCurrentOption(), _mainModel.getSelectedOptionValue());
            } else {
                _lcdView.showOptionsMenu(_mainModel.getSettingsModel().getCurrentOption());
            }
            break;
        case MainStates::WORKING:
            _lcdView.showStatus(_mainModel.getCurrentHumidityResistance(), _mainModel.getSettingsModel().getHumidityThreshold(), _mainModel.getMillisFromLastWatering());
            break;
        case MainStates::WATERING:
            _lcdView.showWatering();
            break;
        case MainStates::PAUSE:
            _lcdView.showPause();
            break;

        default:
            break;
        }

        _lcdViewLastMs = currentMillis;
    }
}
