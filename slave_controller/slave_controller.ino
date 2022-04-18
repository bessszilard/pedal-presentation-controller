//#include <SPI.h>
//#include "printf.h"
//#include "RF24.h"
//#include "Protocol.hpp"
#include "Radio.hpp"

// instantiate an object for the nRF24L01 transceiver
//RF24 radio(A1, A0); // using pin 7 for the CE pin, and pin 8 for the CSN pin
Radio radio(A1, A0, Radio::Mode::Receiver);

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit
bool role = false;  // true = TX role, false = RX role

//ch:PG_DW pg:0001
String commandExample = "ch:PG_DW pg:0001";

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

  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  //  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  //  radio.setPayloadSize(8); // * commandExample.length()); // float datatype occupies 4 bytes
  //
  //  // set the RX address of the TX node into a RX pipe
  //  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1
  //  radio.startListening(); // put radio in RX mode
} // setup

//String cmd;
char cmd[20];

void loop() {
  // This device is a RX node

  //    uint8_t& p_character, uint16_t& p_pageID

  uint8_t pipe;
  if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
    String message;
    Serial.print("pipe?");
    radio.readMessage(message);
    Serial.println("test "+ message );
  }
  //      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
  //      radio.read(cmd, bytes);               // fetch payload from FIFO
  //      Serial.print(F("Received "));
  //      Serial.print(bytes);                    // print the size of the payload
  //      Serial.print(F(" bytes on pipe "));
  //      Serial.print(pipe);                     // print the pipe number
  //      Serial.print(F(": "));
  //      Serial.println(cmd);                // print the payload's value
  //    }
} // loop
