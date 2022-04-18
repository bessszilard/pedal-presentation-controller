//#include <SPI.h>
//#include "printf.h"
//#include "RF24.h"
//#include "Protocol.hpp"
#include "Radio.hpp"

Radio radio(A1, A0, Radio::Mode::Receiver);

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.init()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));
} // setup

void loop() {
  uint8_t pipe;
  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
    String message;
    Serial.print("pipe?");
    radio.readMessage(message);
    Serial.println("test "+ message );
  }
}
