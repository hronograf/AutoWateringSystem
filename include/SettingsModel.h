#ifndef SettingsModel_h
#define SettingsModel_h

#include <stdint.h>

enum class SettingsOption {
    HUMIDITY_THRESHOLD,
    WATERING_DURATION,
    PAUSE_DURATION,
};

class SettingsModel {
private:
    uint32_t _humidityThreshold = 0;

    uint32_t _wateringMs = 3000;
    uint32_t _minWateringMs = 1000;
    uint32_t _maxWateringMs = 999000;

    uint32_t _pauseMs = 10000;
    uint32_t _minPauseMs = 1000;
    uint32_t _maxPauseMs = 999000;

    uint32_t _checkHumidityPeriodMs = 1000;
    const SettingsOption _OPTIONS[3] = {
        SettingsOption::HUMIDITY_THRESHOLD, 
        SettingsOption::WATERING_DURATION,
        SettingsOption::PAUSE_DURATION,
    };
    SettingsOption _currentOption = SettingsOption::HUMIDITY_THRESHOLD;
    bool _isCurrentOptionSelected = false;

    uint8_t getCurrentOptionIndex();

public:
    void next();
    void prev();
    void select();
    void exit();

    SettingsOption getCurrentOption();
    bool isCurrentOptionSelected();

    uint32_t getHumidityThreshold();
    void setHumidityThreshold(uint32_t threshold);
    uint32_t getWateringMs();
    void setWateringMs(uint32_t wateringMs);
    uint32_t getPauseMs();
    void setPauseMs(uint32_t pauseMs);
};

#endif
