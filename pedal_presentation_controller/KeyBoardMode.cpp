#include "KeyBoardMode.hpp"

KeyBoardMode::KeyBoardMode() : m_currentModeIndex(0)
{
  for(int i=0; i<MAX_NUMB_MODES; ++i)
  {
    m_validModes[i] = false;
  }
  
}

void KeyBoardMode::push_back(SingleMode p_newMode)
{
    m_modes[m_currentModeIndex++] = p_newMode;

    if (m_currentModeIndex == MAX_NUMB_MODES) {
      m_currentModeIndex  = 0;
    }
}

String KeyBoardMode::currentModeToString()
{
    return String(m_modes[m_currentModeIndex-1].modeName);
}
