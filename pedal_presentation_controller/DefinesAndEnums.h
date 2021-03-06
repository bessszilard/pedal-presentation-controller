#pragma once
#include <Arduino.h>

#define FEATURE_ENC_A_PIN 5
#define FEATURE_ENC_B_PIN 6
#define FEATURE_ENC_SW_PIN 4
#define KEYBOARD_ENC_A_PIN 9
#define KEYBOARD_ENC_B_PIN 7
#define KEYBOARD_ENC_SW_PIN 8

#define PEDAL_LEFT_PIN A2
#define PEDAL_RIGHT_PIN A3

#define RADIO_CE_PIN A1
#define RADIO_CSN_PIN A0

#define BUTTON_LOGIC 0

#define MAX_NUMB_MODES 35

#define EEPROM_KEYBOARD_MODE_ADDRESS 1
#define EEPROM_WIRE_MODE_ADDRESS 2
#define EEPROM_FEATURE_OPTION_ADDRESS 3
#define EEPROM_STEP_MODE_ADDRESS 4

#define DEBUG 0
#define BLOCK_SERIAL 0

enum StepMode
{
    Pdf,
    Word,
    Count
};