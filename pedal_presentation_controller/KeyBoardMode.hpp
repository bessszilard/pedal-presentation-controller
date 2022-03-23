#include<Arduino.h>
#include <Vector.h>
#include "Pinout.h"

struct SingleMode {
    String modeName;
    String leftCharName;
    int leftChar;
    String rightCharName;
    int rightChar;
};

class KeyBoardMode {
public:
    KeyBoardMode();
    void push_back(SingleMode p_newMode);
    String currentModeToString();
    String currentModeListToString();
    void nextMode();
    void previoustMode();

private:
    int m_currentModeIndex;
    int m_bufferIndex;
    SingleMode m_modes[MAX_NUMB_MODES];
    bool m_validModes[MAX_NUMB_MODES];
};
