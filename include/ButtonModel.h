#include <stdint.h>
#include "Fsm.h"

// enum ButtonState {
//     CHECKED,
//     PROCESSING,
//     PRESSED,
//     HELD,
// };

struct ButtonStates {
    // ButtonStates(State initial, State processing, State pressed, State held);
    State INITIAL;
    State PROCESSING;
    State PRESSED;
    State HELD;
};

class ButtonModel {

private:
    const uint16_t _MILLIS_TO_BECOME_PRESSED;
    const uint16_t _MILLIS_TO_BECOME_HELD;
    const uint8_t _PIN_NUMBER;
    unsigned long _lastSeenMillis;
    uint16_t _highVoltageMillis;
    Fsm _stateMachine;
    ButtonStates _buttonStates;

    void checkVoltage();
    void calcHighVoltageMillis();
    void initializeTimeVars();

public:

    ButtonModel(uint8_t pinNumber, uint16_t millisToBecomePressed, uint16_t millisToBecomeHeld);
    bool isStateReady();
    const State& getState();
    void loopCallback();
    const ButtonStates& getButtonStates();
};
