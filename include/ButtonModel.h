#include <stdint.h>

enum class ButtonState {
    INITIAL,
    PROCESSING,
    PRESSED,
    HELD,
};

class ButtonModel {

private:
    const uint8_t _PIN_NUMBER;
    const uint16_t _MILLIS_TO_BECOME_PRESSED;
    const uint16_t _MILLIS_TO_BECOME_HELD;
    unsigned long _lastSeenMillis;
    uint16_t _highVoltageMillis;
    ButtonState _currentState;

    void processHighVoltageMillis(unsigned long currentMillis);

public:
    ButtonModel(uint8_t pinNumber, uint16_t millisToBecomePressed, uint16_t millisToBecomeHeld);
    bool isStateReady();
    ButtonState getState();
    void loopCallback();
};
