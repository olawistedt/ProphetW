#include "Oscilator.h"
#include <Windows.h>

#define _USE_MATH_DEFINES // To get M_PI
#include <math.h>


//-----------------------------------------------------------------------------------------
// Oscilator
//-----------------------------------------------------------------------------------------

Oscilator::Oscilator()
{
  m_ulSampleRate = 96000;
  m_dFreq = 440.0;
  m_usPeriodLength = (unsigned short)(m_ulSampleRate / m_dFreq);
  setPulseWidth(0.5);      // 100% = +1.0
  m_ucWaveform = kSquare;
  m_dVolume = 0.25;
  m_usCurrent = 0;
}

// Return the next value in stream
double Oscilator::get()
{
  if (!m_bIsOn)
  {
    return 0.0;
  }
  if (m_usCurrent == m_usPeriodLength)
    m_usCurrent = 0; // Start new period.
  m_usCurrent++;

  switch (m_ucWaveform)
  {
  case kSquare:   return getSquare();
  case kSawTooth: return getSawTooth();
//  case kTriangle: return getTriangle();
  case kSine:     return getSine();
  default:        return getSquare();
  }
}

double Oscilator::getSquare()
{
  if (m_usCurrent < m_usPulseWidth)
    return -1.0;
  else
    return +1.0;
}

double Oscilator::getSawTooth()
{
  if (m_usCurrent < m_usPulseWidth || m_usPulseWidth == m_usPeriodLength)
    return (double)(((double)m_usCurrent) * (2.0 / ((double)m_usPulseWidth)) - 1.0);
  else
    return (double)(1.0 - ((double)(m_usCurrent - m_usPulseWidth)) * (2.0 / ((double)(m_usPeriodLength - m_usPulseWidth))));
}

double Oscilator::getSine()
{
  return sin(2.0 * M_PI * ((double)m_usCurrent) / ((double)m_usPeriodLength));
}

//double Oscilator::getTriangle()
//{
//  double phaseIncrement = m_dFreq / m_ulSampleRate;
//  double output = phase * 2.0 - 1.0;
//  phase += phaseIncrement;
//  if (phase > 1.0) {
//    phase -= 2.0;
//  }
//  return output;
//}

void Oscilator::setSampleRate(unsigned long ulSampleRate)
{
  m_ulSampleRate = ulSampleRate;
  m_usPeriodLength = (unsigned short)(m_ulSampleRate / m_dFreq);
  setPulseWidth(m_dPulseWidthInPercent);
}

void Oscilator::setFreq(double fFreq)
{
  m_dFreq = fFreq;
  m_usPeriodLength = (unsigned short)(m_ulSampleRate / m_dFreq);
  setPulseWidth(m_dPulseWidthInPercent);
  m_usCurrent = 0;
}

void Oscilator::setWaveform(unsigned char ucWaveform)
{
  m_ucWaveform = ucWaveform;
}

// Set the pulse width in percent.
// The invalue is a double between 0 and +1.0
void Oscilator::setPulseWidth(double fPulseWidthInPercent)
{
  m_dPulseWidthInPercent = fPulseWidthInPercent;
  m_usPulseWidth = (unsigned short)(m_usPeriodLength * m_dPulseWidthInPercent);
}

// Fill the values array with the period values and return the period lenght.
unsigned short Oscilator::period(double* values)
{
  unsigned short usBackupCurrent = m_usCurrent;
  m_usCurrent = 0;
  for (int i = 0; i < m_usPeriodLength; i++)
  {
    values[i] = get();
  }
  m_usCurrent = usBackupCurrent;
  return m_usPeriodLength;
}
