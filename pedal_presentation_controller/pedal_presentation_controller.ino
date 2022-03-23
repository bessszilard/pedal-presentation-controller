  #include "Keyboard.h"
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <Encoder.h>
  #include "Pinout.h"
  #include "Button.hpp"
  #include "KeyBoardMode.hpp"

  LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
  Encoder myEnc(ENCODER_1_A, ENCODER_1_B);

  Button encoder1Button;
  KeyBoardMode keyBoardMode;


  void setup()
  {
    encoder1Button.configure(ENCODER_1_BUTTON, BUTTON_LOGIC);
    keyBoardMode.push_back(SingleMode{"Arrows", "Left", KEY_LEFT_ARROW, "Right", KEY_RIGHT_ARROW});
    keyBoardMode.push_back(SingleMode{"Pg-DU", "Pg dw", KEY_PAGE_DOWN, "Pg up", KEY_PAGE_UP});
    keyBoardMode.push_back(SingleMode{"Nothing", "L_Pedal", -1, "R_Pedal", -1});

    lcd.init();
    lcd.backlight();
  }


  long oldPosition  = -999;
  int lastState = 0;

  void loop()
  {
    lcd.backlight();
    long newPosition = myEnc.read() / 4;
    if (newPosition != oldPosition)
    {
      newPosition > oldPosition ? keyBoardMode.nextMode() : keyBoardMode.previoustMode();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(keyBoardMode.currentModeListToString());
      lcd.setCursor(0, 1);
      lcd.print("^^^^");

      oldPosition = newPosition;
    }

    if (encoder1Button.isPressed()) 
    {
      // 0 M:PG_DU  PG:1000  
      // 1 W:OFF BT:PG_DW 

      // myEnc.write(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("M:" + keyBoardMode.currentModeToString() + "  PG: 00");
      lcd.setCursor(0, 1);
      lcd.print("W:OFF BT:");
    }
  }
