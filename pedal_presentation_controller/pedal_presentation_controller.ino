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
Radio radio(RADIO_CE_PIN, RADIO_CSN_PIN, Radio::Mode::Transmitter, 20);

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
static bool udpateDefaultL = true;
static bool wirEn = false;      // Wireless enabled
static bool lastWirEn = false;      // Wireless enabled
static bool wirError = false;      // Wireless error
static uint8_t modeIndex = 0;
static uint8_t FeatureModeIndex = 0;

void setup()
{
#if DEBUG
  Serial.begin(115200);
#endif
#if BLOCK_SERIAL
  while(!Serial) {}
#endif

  keyBoardEncButton.configure(KEYBOARD_ENC_SW_PIN, BUTTON_LOGIC);
  featureEncButton.configure(FEATURE_ENC_SW_PIN, BUTTON_LOGIC);

  pedalLeft.configure(PEDAL_LEFT_PIN, BUTTON_LOGIC);
  pedalRight.configure(PEDAL_RIGHT_PIN, BUTTON_LOGIC);
  keyBoardMode.push_back(SingleMode{"Nothing", "Left", 0, "Right", 0, 0});  // default
  keyBoardMode.push_back(SingleMode{"LR_1x", "Left", KEY_LEFT_ARROW, "Right", KEY_RIGHT_ARROW, 1});
  keyBoardMode.push_back(SingleMode{"Pg_UD", "Pg up", KEY_PAGE_UP, "Pg dw", KEY_PAGE_DOWN, 1});
  keyBoardMode.push_back(SingleMode{"UD_7x", "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, 7});
  keyBoardMode.push_back(SingleMode{"UD_8x", "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, 8});
  keyBoardMode.push_back(SingleMode{"UD_9x", "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, 9});

  for(int startId = 10; startId < 31; startId++)
  {
    String lcdText = "UD_" + String(startId) + "x";
    keyBoardMode.push_back(SingleMode{lcdText, "Up", KEY_UP_ARROW, "Down", KEY_DOWN_ARROW, startId});
  }

  lcdLayout.init();
  
  uint8_t wirEnFromMemory;
  persistentMemory.loadConfig(modeIndex, wirEnFromMemory, FeatureModeIndex);
  keyBoardMode.selectMode(modeIndex);
  featureMode.selectMode(FeatureModeIndex);
  wirEn = wirEnFromMemory;

#if DEBUG
  Serial.println("Loaded values from EEPROM");
  Serial.println("Keboard:" + String(keyBoardMode.currentModeToString()) + " " + String(wirEn) + " " + String(featureMode.currentModeToString()));
#endif
  
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
    udpateDefaultL = false;
  }

  if (keyBoardEncButton.isPressed()) 
  {
    persistentMemory.storeKeyboardMode(keyBoardMode.getCurrentModeIndex());
    encoderIdle = true;
    udpateDefaultL = true;
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
    udpateDefaultL = false;
  }

  if (featureEncButton.isPressed()) 
  {
    bool goBack = false;
    featureMode.updateValues(wirEn, pageId, goBack);

    if (goBack)
    {
      keyBoardMode.goToStartPage(pageId);
      radio.sendMessage(keyBoardMode.currentLeftKey(), keyBoardMode.currentRightKey(), pageId);
    }

    persistentMemory.storeWirelessMode((uint8_t)wirEn);
    persistentMemory.storeFeatureMode(featureMode.getCurrentModeIndex());
#if DEBUG
  Serial.println("FeatureMode stored: " + String(featureMode.currentModeToString()));
#endif

    encoderIdle = true;
    udpateDefaultL = true;
  }

  // Radio ---------------------------------------------------
  if(wirEn)
  {
    if (false == radio.isInitialized())
    {
      if (false == radio.init())
        wirError = true;
      else
        wirError = false;
    }
    else if(false == radio.isAvailable() && (false == wirError))
    {
      wirError = true;
      radio.reset();
    }
  }
  if (lastWirEn != wirError)
  {
    udpateDefaultL = true;
    lastWirEn = wirError;
  }

  
#if DEBUG
  Serial.println("radio.isAvailable:" + String(radio.isAvailable()) + "Wir error: " + wirError);
#endif

  if (radio.isInitialized() && wirError)
    radio.reset();
  

  // Pedals --------------------------------------------------
  if (pedalLeft.isPressed()) 
  {
    keyBoardMode.sendCurrentLeftKey(pageId);
    if (wirEn)
      radio.sendMessage(keyBoardMode.currentLeftKey(), keyBoardMode.currentRightKey(), pageId);

    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn, wirError, keyBoardMode.currentLeftKeyToString());
    udpateDefaultL = false;
  }

  if (pedalRight.isPressed()) 
  {
    keyBoardMode.sendCurrentRightKey(pageId);

    if (wirEn)
      radio.sendMessage(keyBoardMode.currentLeftKey(), keyBoardMode.currentRightKey(), pageId);

    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn, wirError, keyBoardMode.currentRightKeyToString());
    udpateDefaultL = false;
  }
  if (pedalLeft.isJustReleased() || pedalRight.isJustReleased())
  {
    udpateDefaultL = true;
  }

  // Default layout -------------------------------------------
  if (udpateDefaultL) 
  {
    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, wirEn, wirError);
    udpateDefaultL = false;
  }
}
