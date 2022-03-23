#include "KeyBoardMode.hpp"

KeyBoardMode::KeyBoardMode() : 
  m_currentModeIndex(0)
  ,m_bufferIndex(0)
{
  for(int i=0; i<MAX_NUMB_MODES; ++i)
  {
    m_validModes[i] = false;
  }
}

void KeyBoardMode::push_back(SingleMode p_newMode)
{
    m_modes[m_bufferIndex++] = p_newMode;

    if (m_bufferIndex == MAX_NUMB_MODES) {
      m_bufferIndex  = MAX_NUMB_MODES;
    }
}

String KeyBoardMode::currentModeListToString()
{
  int tempIndex = m_currentModeIndex;

  String text = m_modes[tempIndex].modeName;
  while(text.length() < 16) {
    tempIndex += 1;
    if (tempIndex == m_bufferIndex) {
      tempIndex = 0;
    }
    text += " " + m_modes[tempIndex].modeName;
  }
  return String(text);
}

String KeyBoardMode::currentModeToString()
{
  return m_modes[m_currentModeIndex].modeName;
}

void KeyBoardMode::nextMode() 
{
  m_currentModeIndex++;
  if (m_bufferIndex <= m_currentModeIndex) 
  {
    m_currentModeIndex = 0;
  }
}

void KeyBoardMode::previoustMode() 
{
  m_currentModeIndex--;
  if (m_currentModeIndex < 0) 
  {
    m_currentModeIndex = m_bufferIndex - 1;
  }
}