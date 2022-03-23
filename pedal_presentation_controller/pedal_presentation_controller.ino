#include <Encoder.h>
#include "Button.hpp"
#include "KeyBoardMode.hpp"
#include "LcdLayouts.hpp"
#include "Pinout.h"

#include "Keyboard.h"

Encoder myEnc(ENCODER_1_A, ENCODER_1_B);

Button encoder1Button;
KeyBoardMode keyBoardMode;
LcdLayouts lcdLayout;

static long oldPosition  = 0;
static int  lastState = 0;
static int  pageId = 0;
static bool encoderIdle = true;
static bool showDefaultL = true;

void setup()
{
  encoder1Button.configure(ENCODER_1_BUTTON, BUTTON_LOGIC);
  keyBoardMode.push_back(SingleMode{"Arrows", "Left", KEY_LEFT_ARROW, "Right", KEY_RIGHT_ARROW});
  keyBoardMode.push_back(SingleMode{"Pg-DU", "Pg dw", KEY_PAGE_DOWN, "Pg up", KEY_PAGE_UP});
  keyBoardMode.push_back(SingleMode{"Nothing", "L_Pedal", -1, "R_Pedal", -1});
  lcdLayout.init();
  myEnc.write(0);
}


void loop()
{
  long newPosition = myEnc.read() / 4;
  if (newPosition != oldPosition)
  {
    if (false == encoderIdle) 
    {
      newPosition > oldPosition ? keyBoardMode.nextMode() : keyBoardMode.previoustMode();
    }

    lcdLayout.selectKeyBoardMode(keyBoardMode.currentModeListToString());
    oldPosition = newPosition;
    encoderIdle = false;
    showDefaultL = false;
  }

  if (encoder1Button.isPressed()) 
  {
    encoderIdle = true;
    showDefaultL = true;
  }

  if (showDefaultL) 
  {
    lcdLayout.defaultL(keyBoardMode.currentModeToString(), pageId, false);
    showDefaultL = false;
  }
}
