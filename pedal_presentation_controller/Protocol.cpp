#include "Protocol.hpp"
#include <Arduino.h>

uint8_t Protocol::GetCheckSum(uint8_t p_payload[5])
{
    uint8_t checkSum = p_payload[0];
    for(int i=1; i<4; ++i)
    {
        checkSum ^= p_payload[i];
    }
    return checkSum;
}

bool Protocol::DecodeMessage(uint8_t p_payload[5], uint8_t& p_keyboardChar, int16_t& p_pageID)
{
    if (p_payload[0] != 'm')
    {
        return false;
    }

    uint8_t checkSum = GetCheckSum(p_payload);
    
    if (p_payload[4] != checkSum)
    {
        return false;
    }

    p_keyboardChar = p_payload[1];
    p_pageID = (p_payload[2] << 8) + p_payload[3];

    return true;
}

bool Protocol::EncodeMessage(uint8_t p_keyboardChar, int16_t p_pageID, uint8_t p_payload[5])
{
    p_payload[0]= 'm';
    p_payload[1] = p_keyboardChar;
    p_payload[2] = p_pageID >> 8;       // MSB
    p_payload[3] = 0xFF & p_pageID;     // LSB
    p_payload[4] = GetCheckSum(p_payload);
    return true;
}

bool Protocol::AsciiByteToStr(uint8_t p_byte, String& p_result) 
{
    if ('!' <= p_byte && p_byte <= '~')
    {
        p_result = String(1, static_cast<char>(p_byte));
        return true;
    }
    
    switch(p_byte)
    {
    case 0x00: p_result = String("NUL "); return true;
    case 0x01: p_result = String("SOH "); return true;
    case 0x02: p_result = String("STX "); return true;
    case 0x03: p_result = String("ETX "); return true;
    case 0x04: p_result = String("EOT "); return true;
    case 0x05: p_result = String("ENQ "); return true;
    case 0x06: p_result = String("ACK "); return true;
    case 0x07: p_result = String("BEL "); return true;
    case 0x08: p_result = String("Backspace"); return true;
    case 0x09: p_result = String("HTAB"); return true;
    case 0x0A: p_result = String("LF "); return true;
    case 0x0B: p_result = String("VT "); return true;
    case 0x0C: p_result = String("FF "); return true;
    case 0x0D: p_result = String("CR "); return true;
    case 0x0E: p_result = String("SO "); return true;
    case 0x0F: p_result = String("SI "); return true;
    case 0x10: p_result = String("DLE"); return true;
    case 0x11: p_result = String("DC1"); return true;
    case 0x12: p_result = String("DC2"); return true;
    case 0x13: p_result = String("DC3"); return true;
    case 0x14: p_result = String("DC4"); return true;
    case 0x15: p_result = String("NAK"); return true;
    case 0x16: p_result = String("SYN"); return true;
    case 0x17: p_result = String("ETB"); return true;
    case 0x18: p_result = String("CAN"); return true;
    case 0x19: p_result = String("EM "); return true;
    case 0x1A: p_result = String("SUB"); return true;
    case 0x1B: p_result = String("ESC"); return true;
    case 0x1C: p_result = String("FS "); return true;
    case 0x1D: p_result = String("GS "); return true;
    case 0x1E: p_result = String("RS "); return true;
    case 0x1F: p_result = String("US "); return true;
    case 0x20: p_result = String("space"); return true;
    case 0x7F: p_result = String("DEL"); return true;
    case 0x80: p_result = String("LEFT_CTRL"); return true;
    case 0x81: p_result = String("LEFT_SHIFT"); return true;
    case 0x82: p_result = String("LEFT_ALT"); return true;
    case 0x83: p_result = String("LEFT_GUI"); return true;
    case 0x84: p_result = String("RIGHT_CTRL"); return true;
    case 0x85: p_result = String("RIGHT_SHIFT"); return true;
    case 0x86: p_result = String("RIGHT_ALT"); return true;
    case 0x87: p_result = String("RIGHT_GUI"); return true;
    case 0xDA: p_result = String("UP_ARROW"); return true;
    case 0xD9: p_result = String("DOWN_ARROW"); return true;
    case 0xD8: p_result = String("LEFT_ARROW"); return true;
    case 0xD7: p_result = String("RIGHT_ARROW"); return true;
    case 0xB2: p_result = String("BACKSPACE"); return true;
    case 0xB3: p_result = String("TAB"); return true;
    case 0xB0: p_result = String("RETURN"); return true;
    case 0xB1: p_result = String("ESC"); return true;
    case 0xD1: p_result = String("INSERT"); return true;
    case 0xD4: p_result = String("DELETE"); return true;
    case 0xD3: p_result = String("PAGE_UP"); return true;
    case 0xD6: p_result = String("PAGE_DOWN"); return true;
    case 0xD2: p_result = String("HOME"); return true;
    case 0xD5: p_result = String("END"); return true;
    case 0xC1: p_result = String("CAPS_LOCK"); return true;
    case 0xC2: p_result = String("F1"); return true;
    case 0xC3: p_result = String("F2"); return true;
    case 0xC4: p_result = String("F3"); return true;
    case 0xC5: p_result = String("F4"); return true;
    case 0xC6: p_result = String("F5"); return true;
    case 0xC7: p_result = String("F6"); return true;
    case 0xC8: p_result = String("F7"); return true;
    case 0xC9: p_result = String("F8"); return true;
    case 0xCA: p_result = String("F9"); return true;
    case 0xCB: p_result = String("F10"); return true;
    case 0xCC: p_result = String("F11"); return true;
    case 0xCD: p_result = String("F12"); return true;
    }
    
    return false;
}