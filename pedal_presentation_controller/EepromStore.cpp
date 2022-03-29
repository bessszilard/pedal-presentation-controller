#include <EEPROM.h>
#include "EepromStore.hpp"
#include "Defines.h"

EepromStore::EepromStore() :
      m_lastStoredMode (0xFF)
    , m_lastStoredwirEn (false)
    , m_memoryWasValid (false)
{
    
}

bool EepromStore::loadConfig(uint8_t &p_mode, uint8_t &p_wirEn)
{
    p_mode = EEPROM.read(EEPROM_KEYBOARD_MODE_ADDRESS);
    p_wirEn = EEPROM.read(EEPROM_WIRE_MODE_ADDRESS);

    // is eeprom valid?
    if (p_mode < MAX_NUMB_MODES && (p_wirEn == 0 || p_wirEn == 1))
    {
        return true;
    }
    return false;
}

void EepromStore::storeWirelessMode(uint8_t p_wirEn)
{
    EEPROM.write(EEPROM_WIRE_MODE_ADDRESS, p_wirEn);
}

void EepromStore::storeKeyboardMode(uint8_t p_mode)
{
    EEPROM.write(EEPROM_KEYBOARD_MODE_ADDRESS, p_mode);
}
