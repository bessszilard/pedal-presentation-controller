#pragma once

#include <Arduino.h>

#define M_CHAR_POS 0
#define LEFT_KEY_CHAR_POS 1
#define RIGHT_KEY_CHAR_POS 2
#define PAGE_ID_POS_MSB 3
#define PAGE_ID_POS_LSB 4
#define CHECK_SUM_POS 5
#define PAYLOAD_SIZE (CHECK_SUM_POS + 1)


class Protocol {
public:
    static uint8_t GetCheckSum(uint8_t p_payload[PAYLOAD_SIZE]);
    static bool DecodeMessage(uint8_t p_payload[PAYLOAD_SIZE], uint8_t& p_leftPedalKey, uint8_t& p_rightPedalKey, int16_t& p_pageID);
    static bool EncodeMessage(uint8_t p_leftPedalKey, uint8_t p_rightPedalKey, int16_t p_pageID, uint8_t p_payload[PAYLOAD_SIZE]);
    static bool AsciiByteToStr(uint8_t p_byte, String& p_result);
};