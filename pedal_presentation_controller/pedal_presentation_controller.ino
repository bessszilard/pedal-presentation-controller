#include <Encoder.h>
#include "Button.hpp"
#include "KeyBoardMode.hpp"
#include "LcdLayouts.hpp"
#include "FeatureModes.hpp"
#include "Defines.h"
#include "EepromStore.hpp"
#include "Keyboard.h"
#include "Radio.hpp"

Encoder keyBoardEnc(KEYBOARD_ENC_A_PIN, KEYBOARD_ENC_B_PIN);
Encoder featureEnc(FEATURE_ENC_A_PIN, FEATURE_ENC_B_PIN);
Radio radio(RADIO_CE_PIN, RADIO_CSN_PIN, Radio::Mode::Transmitter);

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
static int16_t  pageId = 0;
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
  keyBoardMode.push_back(SingleMode{"Nothing", "Left", 0, "Right", 0, 0});  // default
  keyBoardMode.push_back(SingleMode{"Arrows", "Left", KEY_LEFT_ARROW, "Right", KEY_RIGHT_ARROW, 1});
  keyBoardMode.push_back(SingleMode{"Pg-UD", "Pg up", KEY_PAGE_UP, "Pg dw", KEY_PAGE_DOWN, 1});
  keyBoardMode.push_back(SingleMode{"UD_7x", "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, 7});
  keyBoardMode.push_back(SingleMode{"UD_8x", "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, 8});
  keyBoardMode.push_back(SingleMode{"UD_9x", "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, 9});
  lcdLayout.init();
  
  uint8_t wirEnFromMemory;
  persistentMemory.loadConfig(modeIndex, wirEnFromMemory);
  keyBoardMode.selectMode(modeIndex);
  wirEn = wirEnFromMemory;
  
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
    bool goBack = false;
    featureMode.updateValues(wirEn, pageId, goBack);

    if (goBack)
      keyBoardMode.goToStartPage(pageId);

    persistentMemory.storeWirelessMode((uint8_t)wirEn);
    encoderIdle = true;
    showDefaultL = true;
  }

  // Radio ---------------------------------------------------
  if(wirEn && (false == radio.isInitialized()))
  {
    radio.init();
  }
  

  // Pedals --------------------------------------------------
  if (pedalLeft.isPressed()) 
  {
    keyBoardMode.sendCurrentLeftKey(pageId);
    if (wirEn)
      radio.sendMessage(keyBoardMode.currentLeftKey(), pageId);

    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn, keyBoardMode.currentLeftKeyToString());
    showDefaultL = false;
  }

  if (pedalRight.isPressed()) 
  {
    keyBoardMode.sendCurrentRightKey(pageId);

    if (wirEn)
      radio.sendMessage(keyBoardMode.currentRightKey(), pageId);

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
