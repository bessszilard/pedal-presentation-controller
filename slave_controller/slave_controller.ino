#include "Radio.hpp"
#include "Protocol.hpp"
#include "Keyboard.h"

Radio radio(A1, A0, Radio::Mode::Receiver);

bool isSerialAvialable = false;

void setup() {

  Serial.begin(115200);
  isSerialAvialable = Serial;
  //  while (!Serial) {
  //    // some boards need to wait to ensure access to serial over USB
  //  }

  // initialize the transceiver on the SPI bus
  if (!radio.init()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));
} // setup

int16_t originalPageId = 0;
uint8_t leftPedalChar = 0;
uint8_t rightPedalChar = 0;

void loop() {
  uint8_t pipe;
  if (radio.available(&pipe))             // is there a payload? get the pipe number that recieved it
  {
    int16_t receivedPagId;
    if (radio.readMessage(leftPedalChar, rightPedalChar, receivedPagId))
    {
      String leftCharStr, rightCharStr ;
      Protocol::AsciiByteToStr(leftPedalChar, leftCharStr);
      Protocol::AsciiByteToStr(rightPedalChar, rightCharStr);
      Serial.println("Rec:" + leftCharStr + " " + rightCharStr + " PgID:" + String(receivedPagId));

      if (originalPageId < receivedPagId)
      {
        while (originalPageId < receivedPagId)
        {
          Keyboard.press(rightPedalChar);
          Keyboard.releaseAll();
          originalPageId++;
        }
      }
      else if (receivedPagId < originalPageId )
      {
        while (receivedPagId < originalPageId)
        {
          Keyboard.press(leftPedalChar);
          Keyboard.releaseAll();
          originalPageId--;
          if (originalPageId < 0)
          {
            originalPageId = 0;
          }
        }
      }

    }
    else {
      Serial.println("corrupt message");
    }
  }
}
