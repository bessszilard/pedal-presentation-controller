#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DefinesAndEnums.h"
class LcdLayouts {
public:
    LcdLayouts();

    void init();

    void selectKeyBoardMode(const String& p_firstLine);
    void defaultL(const String& p_mode, int p_pageId, bool p_wireless, bool p_wirelessError, StepMode p_stepMode, const String& p_buttonText = "");
private:
    LiquidCrystal_I2C m_lcd;
};