#include <Arduino.h>
#include "DefinesAndEnums.h"

struct SingleMode {
    String modeName;
    char* leftCharName;
    int leftChar;
    char* rightCharName;
    int rightChar;
    int sendMultipleTimes;
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

    void sendCurrentRightKey(int16_t& p_pageId);
    void sendCurrentLeftKey(int16_t& p_pageId);

    void nextMode();
    void previoustMode();
    void goToStartPage(int16_t& p_pageId);
    void updateStepMode(StepMode p_stepMode);

    inline int getCurrentModeIndex() const {
        return m_currentModeIndex;
    }

private:
    int m_initialized;
    int m_currentModeIndex;
    int m_minNameLength;
    int m_bufferIndex;
    StepMode m_stepMode;
    SingleMode m_modes[MAX_NUMB_MODES];
};
