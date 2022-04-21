#include<Arduino.h>
#include <Vector.h>
#include "Defines.h"

enum class FeatureActionEnum 
{
    WirelessOn = 0,
    WirelessOff,
    ResetPageId,
    ResetPageIdAndGoBack,
    Count
};


struct FeatureMode {
    String modeName;
    FeatureActionEnum action;
};

class FeatureModes {
public:
    FeatureModes();
    void push_back(FeatureMode p_newMode);
    String currentModeToString();
    String currentModeListToString();

    void nextMode();
    void previoustMode();
    void updateValues(bool& p_wirelessMode, int16_t& p_pageId, bool& p_goBack);

private:
    int m_currentModeIndex;
    int m_minNameLength;
    int m_bufferIndex;
    FeatureMode m_modes[(int)FeatureActionEnum::Count];
};
