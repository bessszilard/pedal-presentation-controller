#include <Encoder.h>
#include "Button.hpp"
#include "KeyBoardMode.hpp"
#include "LcdLayouts.hpp"
#include "FeatureModes.hpp"
#include "Defines.h"

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

static long keOldPosition  = 0;
static long feOldPosition  = 0;
static int  lastState = 0;
static int  pageId = 0;
static bool encoderIdle = true;
static bool showDefaultL = true;
static bool wirelessEnabled = false;

void setup()
{
  keyBoardEncButton.configure(KEYBOARD_ENC_SW_PIN, BUTTON_LOGIC);
  featureEncButton.configure(FEATURE_ENC_SW_PIN, BUTTON_LOGIC);

  pedalLeft.configure(PEDAL_LEFT_PIN, BUTTON_LOGIC);
  pedalRight.configure(PEDAL_RIGHT_PIN, BUTTON_LOGIC);
  keyBoardMode.push_back(SingleMode{"Arrows", "Left", KEY_LEFT_ARROW, "Right", KEY_RIGHT_ARROW});
  keyBoardMode.push_back(SingleMode{"Pg-DU", "Pg dw", KEY_PAGE_DOWN, "Pg up", KEY_PAGE_UP});
  keyBoardMode.push_back(SingleMode{"Nothing", "Left", -1, "Right", -1});
  lcdLayout.init();

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
    encoderIdle = true;
    showDefaultL = true;
  }

  // Keyboard Mode encoder -------------------------------------------
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
    featureMode.updateValues(wirelessEnabled, pageId);
    encoderIdle = true;
    showDefaultL = true;
  }



  // Pedals --------------------------------------------------
  if (pedalLeft.isPressed()) 
  {
    pageId -= 1;
    if (pageId < 0)
    {
      pageId = 0;
    }
    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirelessEnabled, keyBoardMode.currentLeftKeyToString());
    showDefaultL = false;
  }

  if (pedalRight.isPressed()) 
  {
    pageId += 1;
    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirelessEnabled, keyBoardMode.currentRightKeyToString());
    showDefaultL = false;
  }
  if (pedalLeft.isJustReleased() || pedalRight.isJustReleased())
  {
    showDefaultL = true;
  }

  // Default layout -------------------------------------------
  if (showDefaultL) 
  {
    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirelessEnabled);
    showDefaultL = false;
  }
}
