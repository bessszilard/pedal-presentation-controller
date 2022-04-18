#pragma once

#include <Arduino.h>

class Protocol {
public:
    static uint8_t GetCheckSum(uint8_t p_payload[5]);
    static bool DecodeMessage(uint8_t p_payload[5], uint8_t& p_keyboardChar, int16_t& p_pageID);
    static bool EncodeMessage(uint8_t p_keyboardChar, int16_t p_pageID, uint8_t p_payload[5]);
    static bool AsciiByteToStr(uint8_t p_byte, String& p_result);
};