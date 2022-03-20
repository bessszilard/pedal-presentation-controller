#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "Pinout.h"
#include "Button.hpp"

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
Encoder myEnc(ENCODER_1_A, ENCODER_1_B);

Button encoder1Button;

void setup()
{
  encoder1Button.configure(ENCODER_1_BUTTON, BUTTON_LOGIC);
  
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
    lcd.clear();
    lcd.setCursor(0, 0);
    String posString = "Rot: " + String(newPosition);
    lcd.print(posString);
    oldPosition = newPosition;
  }

  int state = encoder1Button.currentState();
  if (state != lastState) 
  {
    lcd.setCursor(0, 1);
    String btnString = "State: " + String(state);
    lcd.print(btnString);
    lastState = state;
  }
  

  if (encoder1Button.isPressed()) 
  {
    myEnc.write(0);
    String posString = "Rot: " + String(myEnc.read());
    lcd.print(posString);
  }
}
