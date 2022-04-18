#pragma one 
#include <Arduino.h>

class RF24;

class Radio {
public:
    enum class Mode
    {
        Invalid,
        Transmitter,
        Receiver
    };

    Radio(int p_CE_pin, int p_CSN_pin, Mode p_radioMode, uint8_t p_payLoad = 6);
    ~Radio();

    bool init();
    bool available(uint8_t* p_pipe);
    bool readMessage(uint8_t& p_character, uint16_t& p_pageID);
    bool readMessage(String& p_message);
    bool sendMessage(uint8_t p_keyboardChar, int16_t p_pageID);

    inline bool isInitialized() { return m_initialized; }
private:
    RF24* m_nrf24;
    bool m_initialized;
    Mode m_radioMode;
    
};