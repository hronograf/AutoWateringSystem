#ifndef Controller_h
#define Controller_h

#include <Arduino.h>
#include "MainModel.h"
#include "LCDView.h"

class Controller {
private:
    MainModel _mainModel = MainModel();
    LCDView _lcdView = LCDView();
    unsigned long _mainModelLastMs = millis();
    uint8_t _mainModelLoopPeriodMs = 10;
    unsigned long _lcdViewLastMs = millis();
    uint8_t _lcdViewLoopPeriodMs = 100;

public:
    void setup();
    void loop();
};

#endif
