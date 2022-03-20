#include "Button.hpp"
#include <Arduino.h>

Button::Button() {}

void Button::configure(int p_pin, int p_logic)
{
    m_pin = p_pin;
    m_logic = p_logic;
    pinMode(m_pin, INPUT);
    m_prevState = p_logic;
}

bool Button::isPressed()
{
    int state = digitalRead(m_pin);
    if ((state != m_prevState) && (state == m_logic))
    {
        delay(50);
        state = digitalRead(m_pin);
        if ((state != m_prevState) && (state == m_logic))
        {
            m_prevState = state;
            return true;
        }
        m_prevState = state;
        return false;
    }
    m_prevState = state;
    return false;
}

bool Button::currentState() 
{
    return digitalRead(m_pin);
}