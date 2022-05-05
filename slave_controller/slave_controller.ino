#include "Radio.hpp"
#include "Protocol.hpp"
#include "Keyboard.h"

#define DEBUG 1

Radio radio(A1, A0, Radio::Mode::Receiver);

bool isSerialAvialable = false;

void setup() {
#if DEBUG
  Serial.begin(115200);
  isSerialAvialable = Serial;
  while (!Serial) {
  //    // some boards need to wait to ensure access to serial over USB
  }
#endif

  // initialize the transceiver on the SPI bus
  if (!radio.init()) {
    #if DEBUG
    Serial.println(F("radio hardware is not responding!!"));
    #endif
    while (1) {} // hold in infinite loop
  }

  #if DEBUG
  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));
  #endif
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
#if DEBUG
      Serial.println("Rec:" + leftCharStr + " " + rightCharStr + " PgID:" + String(receivedPagId));
#endif
      if (originalPageId < receivedPagId)
      {
        while (originalPageId < receivedPagId)
        {
          Serial.println("Go forward:" + String(receivedPagId - originalPageId ));
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
#if DEBUG
      Serial.println("corrupt message");
#endif
    }
  }
}
