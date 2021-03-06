

class EepromStore {
public:
    EepromStore();
    void loadConfig(uint8_t &p_mode, uint8_t &p_wirEn, uint8_t &p_featureMode, uint8_t &p_stepMode);
    void storeWirelessMode(uint8_t p_mode);
    void storeKeyboardMode(uint8_t p_wirEn);
    void storeFeatureMode(uint8_t p_FeatureMode);
    void storeStepMode(uint8_t p_mode);

private:
    uint8_t m_lastStoredMode;
    bool    m_lastStoredwirEn;
    bool    m_memoryWasValid;
};