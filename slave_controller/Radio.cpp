#include "Radio.hpp"
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "Protocol.hpp"

Radio::Radio(int p_CE_pin, int p_CSN_pin, Mode p_radioMode, uint8_t p_payLoad /* = 6*/) :
      m_radioMode(p_radioMode)
    , m_nrf24 {new RF24(p_CE_pin, p_CSN_pin)}
    , m_initialized(false)
{
}

Radio::~Radio()
{
    delete m_nrf24;
}

bool Radio::init()
{
    if(m_initialized)
    {
        return true;
    }
    
    bool radioStarted = m_nrf24->begin();
    uint8_t address[6] = "1Node";

    m_nrf24->setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
    m_nrf24->setPayloadSize(PAYLOAD_SIZE);  // * commandExample.length()); // float datatype occupies 4 bytes

    switch(m_radioMode)
    {
        case Mode::Receiver:
            m_nrf24->openReadingPipe(1, address); // using pipe 1
            m_nrf24->startListening(); // put radio in RX mode
            break;

        case Mode::Transmitter:
            // set the TX address of the RX node into the TX pipe
            m_nrf24->openWritingPipe(address);     // always uses pipe 0
            m_nrf24->stopListening();  // put radio in TX mode
            break;
    }
    if (radioStarted)
    {
        m_initialized = true;
    }
    return radioStarted;
}

bool Radio::available(uint8_t* p_pipe)
{
    return m_nrf24->available(p_pipe);
}

bool Radio::readMessage(uint8_t& p_leftPedalKey, uint8_t& p_rightPedalKey, int16_t& p_pageID)
{
    char cmd[PAYLOAD_SIZE];
    uint8_t bytes = m_nrf24->getPayloadSize(); // get the size of the payload
    m_nrf24->read(cmd, bytes);               // fetch payload from FIFO

    if(false == Protocol::DecodeMessage(cmd, p_leftPedalKey, p_rightPedalKey, p_pageID)) {
        // checkSum error
        return false;
    }

    return true;
}

bool Radio::readMessage(String& p_message)
{
    uint8_t cmd[PAYLOAD_SIZE];
    uint8_t bytes = m_nrf24->getPayloadSize(); // get the size of the payload
    m_nrf24->read(cmd, bytes);               // fetch payload from FIFO

    uint8_t leftPedalKey;
    uint8_t rightPedalKey;
    int16_t pageId;

    if(false == Protocol::DecodeMessage(cmd, leftPedalKey, rightPedalKey, pageId)) {
        // checkSum error
        return false;
    }

    String leftKeyStr;
    String rightKeyStr;
    Protocol::AsciiByteToStr(leftPedalKey, leftKeyStr);
    Protocol::AsciiByteToStr(rightPedalKey, rightKeyStr);
    p_message = String(static_cast<char>(cmd[0])) + " " + leftKeyStr + " " + rightKeyStr + " " + pageId;
    return true;
}

bool Radio::sendMessage(uint8_t p_leftPedalKey, uint8_t p_rightPedalKey, int16_t p_pageID)
{
    uint8_t txPayload[PAYLOAD_SIZE];
    Protocol::EncodeMessage(p_leftPedalKey, p_rightPedalKey, p_pageID, txPayload);
    return m_nrf24->write(txPayload, PAYLOAD_SIZE);    // transmit & save the report
}