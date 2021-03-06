#include <EEPROM.h>
#include "EepromStore.hpp"
#include "DefinesAndEnums.h"

EepromStore::EepromStore() :
      m_lastStoredMode (0xFF)
    , m_lastStoredwirEn (false)
    , m_memoryWasValid (false)
{
    
}

void EepromStore::loadConfig(uint8_t &p_mode, uint8_t &p_wirEn, uint8_t &p_featureMode, uint8_t &p_stepMode)
{
    p_mode = EEPROM.read(EEPROM_KEYBOARD_MODE_ADDRESS);
    p_wirEn = EEPROM.read(EEPROM_WIRE_MODE_ADDRESS);
    p_featureMode = EEPROM.read(EEPROM_FEATURE_OPTION_ADDRESS);
    p_stepMode = EEPROM.read(EEPROM_STEP_MODE_ADDRESS);

    // if invalid, load with default
    if (MAX_NUMB_MODES < p_mode)
        p_mode = 0;

    // if invalid, load with default
    if (MAX_NUMB_MODES < p_featureMode)
        p_featureMode = 0;

    if (p_wirEn != 0 && p_wirEn != 1)
        p_wirEn = 0; 
        
    if ((int)StepMode::Count < p_stepMode)
        p_stepMode = 0;
}

void EepromStore::storeWirelessMode(uint8_t p_wirEn)
{
    uint8_t wirEn = EEPROM.read(EEPROM_KEYBOARD_MODE_ADDRESS);
    if (wirEn == p_wirEn)
    {
        return;
    }
    EEPROM.write(EEPROM_WIRE_MODE_ADDRESS, p_wirEn);
}

void EepromStore::storeKeyboardMode(uint8_t p_mode)
{
    uint8_t mode = EEPROM.read(EEPROM_KEYBOARD_MODE_ADDRESS);
    if (mode == p_mode)
    {
        return;
    }
    EEPROM.write(EEPROM_KEYBOARD_MODE_ADDRESS, p_mode);
}

void EepromStore::storeFeatureMode(uint8_t p_mode)
{
    uint8_t mode = EEPROM.read(EEPROM_FEATURE_OPTION_ADDRESS);
    if (mode == p_mode)
    {
        return;
    }
    EEPROM.write(EEPROM_FEATURE_OPTION_ADDRESS, p_mode);
}

void EepromStore::storeStepMode(uint8_t p_mode)
{
    uint8_t mode = EEPROM.read(EEPROM_STEP_MODE_ADDRESS);
    if (mode == p_mode)
    {
        return;
    }
    EEPROM.write(EEPROM_STEP_MODE_ADDRESS, p_mode);
}
