#include<Arduino.h>
#include <Vector.h>
#include "Defines.h"

struct SingleMode {
    String modeName;
    String leftCharName;
    int leftChar;
    String rightCharName;
    int rightChar;
    bool sendKey;
};

class KeyBoardMode {
public:
    KeyBoardMode();
    void push_back(SingleMode p_newMode);
    String currentModeToString();
    String currentModeListToString();
    String currentLeftKeyToString();
    String currentRightKeyToString();

    void sendCurrentRightKey();
    void sendCurrentLeftKey();

    void nextMode();
    void previoustMode();

private:
    int m_initialized;
    int m_currentModeIndex;
    int m_minNameLength;
    int m_bufferIndex;
    SingleMode m_modes[MAX_NUMB_MODES];
};
