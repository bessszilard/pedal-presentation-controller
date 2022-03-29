#include <Encoder.h>
#include "Button.hpp"
#include "KeyBoardMode.hpp"
#include "LcdLayouts.hpp"
#include "FeatureModes.hpp"
#include "Defines.h"
#include "EepromStore.hpp"
#include "Keyboard.h"

Encoder keyBoardEnc(KEYBOARD_ENC_A_PIN, KEYBOARD_ENC_B_PIN);
Encoder featureEnc(FEATURE_ENC_A_PIN, FEATURE_ENC_B_PIN);

Button keyBoardEncButton;
Button featureEncButton;
Button pedalLeft;
Button pedalRight;
KeyBoardMode keyBoardMode;
FeatureModes featureMode;
LcdLayouts lcdLayout;
EepromStore persistentMemory;

static long keOldPosition  = 0;
static long feOldPosition  = 0;
static int  pageId = 0;
static bool encoderIdle = true;
static bool showDefaultL = true;
static bool wirEn = false;      // Wireless enabled
static uint8_t modeIndex = 0;

void setup()
{
  keyBoardEncButton.configure(KEYBOARD_ENC_SW_PIN, BUTTON_LOGIC);
  featureEncButton.configure(FEATURE_ENC_SW_PIN, BUTTON_LOGIC);

  pedalLeft.configure(PEDAL_LEFT_PIN, BUTTON_LOGIC);
  pedalRight.configure(PEDAL_RIGHT_PIN, BUTTON_LOGIC);
  keyBoardMode.push_back(SingleMode{"Arrows", "Left", KEY_LEFT_ARROW, "Right", KEY_RIGHT_ARROW, true});
  keyBoardMode.push_back(SingleMode{"Pg-UD", "Pg up", KEY_PAGE_UP, "Pg dw", KEY_PAGE_DOWN, true});
  keyBoardMode.push_back(SingleMode{"Nothing", "Left", 0, "Right", 0, false});
  lcdLayout.init();
  
  uint8_t wirEnFromMemory;
  if (persistentMemory.loadConfig(modeIndex, wirEnFromMemory))
  {
    keyBoardMode.selectMode(modeIndex);
    wirEn = wirEnFromMemory;
  }
  else
  {
    modeIndex = 0;
    wirEn = false;
  }

  keyBoardEnc.write(0);
  featureEnc.write(0);
}


void loop()
{
  // Keyboard Mode encoder -------------------------------------------
  long newPosition = keyBoardEnc.read() / 4;
  if (newPosition != keOldPosition)
  {
    if (false == encoderIdle) 
    {
      newPosition > keOldPosition ? keyBoardMode.nextMode() : keyBoardMode.previoustMode();
    }

    lcdLayout.selectKeyBoardMode(keyBoardMode.currentModeListToString());
    keOldPosition = newPosition;
    encoderIdle = false;
    showDefaultL = false;
  }

  if (keyBoardEncButton.isPressed()) 
  {
    persistentMemory.storeKeyboardMode(keyBoardMode.getCurrentModeIndex());

    encoderIdle = true;
    showDefaultL = true;
  }

  // Feature Mode encoder -------------------------------------------
  newPosition = featureEnc.read() / 4;
  if (newPosition != feOldPosition)
  {
    if (false == encoderIdle) 
    {
      newPosition > feOldPosition ? featureMode.nextMode() : featureMode.previoustMode();
    }

    lcdLayout.selectKeyBoardMode(featureMode.currentModeListToString());
    feOldPosition = newPosition;
    encoderIdle = false;
    showDefaultL = false;
  }

  if (featureEncButton.isPressed()) 
  {
    featureMode.updateValues(wirEn, pageId);

    persistentMemory.storeWirelessMode((uint8_t)wirEn);
    encoderIdle = true;
    showDefaultL = true;
  }



  // Pedals --------------------------------------------------
  if (pedalLeft.isPressed()) 
  {
    pageId = pageId == 0 ? 0 : pageId - 1;
    keyBoardMode.sendCurrentLeftKey();

    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn, keyBoardMode.currentLeftKeyToString());
    showDefaultL = false;
  }

  if (pedalRight.isPressed()) 
  {
    pageId += 1;
    keyBoardMode.sendCurrentRightKey();

    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn, keyBoardMode.currentRightKeyToString());
    showDefaultL = false;
  }
  if (pedalLeft.isJustReleased() || pedalRight.isJustReleased())
  {
    showDefaultL = true;
  }

  // Default layout -------------------------------------------
  if (showDefaultL) 
  {
    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn);
    showDefaultL = false;
  }
}
