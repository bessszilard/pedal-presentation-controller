#include <SPI.h>
#include "printf.h"
#include "RF24.h"

// CE, CSN pins
RF24 radio(A1, A0);

uint8_t address[][6] = {"1Node", "2Node"};

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = true;  // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float payload = 0.0;

//ch:PG_DW pg:0001
String command = "ch:PG_DW pg:0001";

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));

  // char input = Serial.parseInt();
  // radioNumber = input == 1;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  radio.setPayloadSize(sizeof(command));

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0
  radio.stopListening();  // put radio in TX mode

  // For debugging info
  // printf_begin();             // needed only once for printing details
//  radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data

} // setup

void loop() {

  if (role) {
    // This device is a TX node

    unsigned long start_timer = micros();                    // start the timer
//    bool report = radio.write(&command, sizeof(command));    // transmit & save the report
    bool report = radio.write(&command, sizeof(command) * command.length());    // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(payload);                               // print payload sent
      payload += 0.01;                                       // increment float payload
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }

    // to make this example readable in the serial monitor
    delay(1000);  // slow transmissions down by 1 second
  }
} // loop
