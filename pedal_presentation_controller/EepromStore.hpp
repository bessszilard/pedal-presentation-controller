

class EepromStore {
public:
    EepromStore();
    bool loadConfig(uint8_t &p_mode, uint8_t &p_wirEn);
    void storeWirelessMode(uint8_t p_mode);
    void storeKeyboardMode(uint8_t p_wirEn);

private:
    uint8_t m_lastStoredMode;
    bool    m_lastStoredwirEn;
    bool    m_memoryWasValid;
};