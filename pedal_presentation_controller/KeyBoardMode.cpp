#include "Keyboard.h"
#include "KeyBoardMode.hpp"

KeyBoardMode::KeyBoardMode() : 
  m_currentModeIndex(0)
  , m_bufferIndex(0)
  , m_minNameLength(5)
{
}

void KeyBoardMode::push_back(SingleMode p_newMode)
{
  if (false == m_initialized)
  {
    Keyboard.begin();
    m_initialized = true;
  }
    m_modes[m_bufferIndex] = p_newMode;

    int mNameL = m_modes[m_bufferIndex].modeName.length();

    if (mNameL < m_minNameLength) {
      for (int i = 0; i < (m_minNameLength - mNameL); ++i) {
        m_modes[m_bufferIndex].modeName += " ";
      }
    }

    m_bufferIndex++;
    if (m_bufferIndex == MAX_NUMB_MODES) {
      m_bufferIndex  = MAX_NUMB_MODES;
    }
}

void KeyBoardMode::selectMode( int p_modeIndex )
{
  if (p_modeIndex < m_bufferIndex)
  {
    m_currentModeIndex = p_modeIndex;
  }
}

String KeyBoardMode::currentModeListToString()
{
  // ------- LCD layout -------
  // 0123456789ABCDEF
  // ODE0 MODE1 MODE2
  //      ^^^^
  // --------------------------

  int tempIndex = m_currentModeIndex;

  String lcdLine = m_modes[tempIndex].modeName;

  while(lcdLine.length() < 12) {
    tempIndex += 1;
    if (tempIndex == m_bufferIndex) {
      tempIndex = 0;
    }
    lcdLine += " " + m_modes[tempIndex].modeName;
  }

  tempIndex = m_currentModeIndex - 1; // previous one
  if (tempIndex < 0) 
  {
    tempIndex = m_bufferIndex - 1;
  }

  String tempString = m_modes[tempIndex].modeName;
  lcdLine = tempString.substring(tempString.length() - m_minNameLength + 1) + " " + lcdLine;

  return String(lcdLine);

}

String KeyBoardMode::currentModeToString()
{
  return m_modes[m_currentModeIndex].modeName;
}

String KeyBoardMode::currentLeftKeyToString()
{
  return m_modes[m_currentModeIndex].leftCharName;
}

String KeyBoardMode::currentRightKeyToString()
{
  return m_modes[m_currentModeIndex].rightCharName;
}

void KeyBoardMode::sendCurrentRightKey()
{
  if (m_modes[m_currentModeIndex].sendKey) {
    Keyboard.press(m_modes[m_currentModeIndex].rightChar);
    Keyboard.releaseAll();
  }
}

void KeyBoardMode::sendCurrentLeftKey()
{
  if (m_modes[m_currentModeIndex].sendKey) {
    Keyboard.press(m_modes[m_currentModeIndex].leftChar);
    Keyboard.releaseAll();
  }
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