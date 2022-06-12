#include "FeatureModes.hpp"

FeatureModes::FeatureModes() : 
  m_currentModeIndex(0)
  , m_bufferIndex(0)
  , m_minNameLength(5)
{
  push_back(FeatureMode {"Wir-Off", FeatureActionEnum::WirelessOn});
  push_back(FeatureMode {"Wir-On", FeatureActionEnum::WirelessOff});
  push_back(FeatureMode {"Rst-pgNum", FeatureActionEnum::ResetPageId});
  push_back(FeatureMode {"Rst+GoBack", FeatureActionEnum::ResetPageIdAndGoBack});
  push_back(FeatureMode {"Pdf", FeatureActionEnum::SetPdfStepMode});
  push_back(FeatureMode {"Word", FeatureActionEnum::SetWordStepMode});
}

void FeatureModes::push_back(FeatureMode p_newMode)
{
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

String FeatureModes::currentModeListToString()
{
  // 0123456789ABCDEF
  // ODE0 MODE1 MODE2
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

String FeatureModes::currentModeToString()
{
  return m_modes[m_currentModeIndex].modeName;
}

void FeatureModes::nextMode() 
{
  m_currentModeIndex++;
  if (m_bufferIndex <= m_currentModeIndex) 
  {
    m_currentModeIndex = 0;
  }
}

void FeatureModes::previoustMode() 
{
  m_currentModeIndex--;
  if (m_currentModeIndex < 0) 
  {
    m_currentModeIndex = m_bufferIndex - 1;
  }
}

void FeatureModes::updateValues(bool& p_wirelessMode, int16_t& p_pageId, bool& p_goBack, StepMode& p_stepMode)
{
  p_goBack = false;
  switch(m_modes[m_currentModeIndex].action)
  {
    case FeatureActionEnum::WirelessOn:           p_wirelessMode = false;              break;
    case FeatureActionEnum::WirelessOff:          p_wirelessMode = true;               break;
    case FeatureActionEnum::ResetPageId:          p_pageId = 0;                        break;
    case FeatureActionEnum::ResetPageIdAndGoBack: p_goBack = true;                     break;
    case FeatureActionEnum::SetPdfStepMode:   p_stepMode = StepMode::Pdf;  break;
    case FeatureActionEnum::SetWordStepMode:  p_stepMode = StepMode::Word; break;
  }
}

void FeatureModes::selectMode( int p_modeIndex )
{
  if (p_modeIndex < m_bufferIndex)
  {
    m_currentModeIndex = p_modeIndex;
  }
}
