#include "Radio.hpp"
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "Protocol.hpp"

#define PAYLOAD_SIDE 5

Radio::Radio(int p_CE_pin, int p_CSN_pin, Mode p_radioMode, uint8_t p_payLoad /* = 6*/) :
    m_radioMode(p_radioMode)
    , m_nrf24 {new RF24(p_CE_pin, p_CSN_pin)}
{
    // m_address = {"2Node"};
}

Radio::~Radio()
{
    delete m_nrf24;
}

bool Radio::init()
{
    bool radioStarted = m_nrf24->begin();
    uint8_t address[6] = "1Node";

    m_nrf24->setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
    m_nrf24->setPayloadSize(PAYLOAD_SIDE);  // * commandExample.length()); // float datatype occupies 4 bytes

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

    return radioStarted;
    //TODOsz connect
}

bool Radio::available(uint8_t* p_pipe)
{
    return m_nrf24->available(p_pipe);
}

bool Radio::readMessage(uint8_t& p_character, uint16_t& p_pageID)
{
    char cmd[PAYLOAD_SIDE];
    uint8_t bytes = m_nrf24->getPayloadSize(); // get the size of the payload
    m_nrf24->read(cmd, bytes);               // fetch payload from FIFO

}

bool Radio::readMessage(String& p_message)
{
    uint8_t cmd[PAYLOAD_SIDE];
    uint8_t bytes = m_nrf24->getPayloadSize(); // get the size of the payload
    m_nrf24->read(cmd, bytes);               // fetch payload from FIFO

    uint8_t keyBoarChar;
    int16_t pageId;

    Protocol::DecodeMessage(cmd, keyBoarChar, pageId);
    String keyStr;
    Protocol::AsciiByteToStr(keyBoarChar, keyStr);
    p_message = String(cmd[0]) + " " + keyStr + " " + pageId;
}

bool Radio::sendMessage(uint8_t p_keyboardChar, int16_t p_pageID)
{
    uint8_t txPayload[PAYLOAD_SIDE];
    Protocol::EncodeMessage(p_keyboardChar, p_pageID, txPayload);
    return m_nrf24->write(txPayload, PAYLOAD_SIDE);    // transmit & save the report
}