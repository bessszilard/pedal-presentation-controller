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
    void selectMode( int p_modeIndex );

    String currentModeToString();
    String currentModeListToString();
    String currentLeftKeyToString();
    String currentRightKeyToString();
    int currentLeftKey();
    int currentRightKey();

    void sendCurrentRightKey();
    void sendCurrentLeftKey();

    void nextMode();
    void previoustMode();

    inline int getCurrentModeIndex() const {
        return m_currentModeIndex;
    }

private:
    int m_initialized;
    int m_currentModeIndex;
    int m_minNameLength;
    int m_bufferIndex;
    SingleMode m_modes[MAX_NUMB_MODES];
};
