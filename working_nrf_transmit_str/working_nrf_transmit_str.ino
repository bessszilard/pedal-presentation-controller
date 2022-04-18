#include "Radio.hpp"

Radio radio(A1, A0, Radio::Mode::Transmitter);

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
}

void loop() {

  if (role) {
    uint8_t txPayload[5];
    uint8_t keyboardChar = 0x85;
    int16_t pageId = 101;
    
    unsigned long start_timer = micros();                    // start the timer
    bool report = radio.sendMessage(keyboardChar, pageId);    // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(command);                               // print payload sent
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      Serial.println(command);                               // print payload sent
    }

    // to make this example readable in the serial monitor
    delay(1000);  // slow transmissions down by 1 second
  }
} // loop
