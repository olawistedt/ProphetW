///////////////////////////////////////////////////////////////////////////////
//
//
//                 
//                /:\
//               / :  \
//              /  :    \
//             /   :      \
//            /    :        \_____________..._____
//           /     :         :                    :\
//          /      :         :      |             :  \
//         /       :         :      S             :    \
//        /        :         :      |             :      \
//        <---A---> <---D--->                      <--R-->
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Envelope.h"

Envelope::Envelope()
{
  m_ucInPhase = kIdle;

  m_fAttack   = 100;  // Msek
  m_fDecay    = 100;  // Msek
  m_fSustain  = 0.5;
  m_fRelease  = 1000; // Msek
}
void  Envelope::restart()
{
  m_ulCurrent = 0;
  m_ucInPhase = kAttackPhase;
}
void  Envelope::beginReleasePhase()
{
  m_ulCurrent = 0;
  m_ucInPhase = kReleasePhase;
}
void  Envelope::setSampleRate(unsigned long ulNewSampleRate)
{
  m_ulSampleRate         = ulNewSampleRate;
  m_fMsecsBetweenSamples = (double)(1000.0 / ((double)m_ulSampleRate)); // 1 sampleFrame in 96kb/sek = 0.0104 millisec
  setAttack  (m_fAttack);
  setDecay   (m_fDecay);
  setRelease (m_fRelease);
}
void  Envelope::setAttack  (double fMsec)
{
  m_fAttack  = fMsec;
  m_ulAttackEnds = (long)(m_fAttack / m_fMsecsBetweenSamples);
}
void  Envelope::setDecay   (double fMsec)
{
  m_fDecay      = fMsec;
  m_ulDecayEnds = (long)(m_fDecay / m_fMsecsBetweenSamples);
}
void  Envelope::setSustain (double fLevel)
{
  m_fSustain = fLevel;
}
void  Envelope::setRelease(double fMsec)
{
  m_fRelease      = fMsec;
  m_ulReleaseEnds = (long)(m_fRelease / m_fMsecsBetweenSamples);
}
double Envelope::get()
{
//  static bool ba,bd,bs,br = false;

  double fRet = 1.0;

  static bool isBypassEnvelope = false;
  if (isBypassEnvelope)
  {
    if (m_ucInPhase == kReleasePhase)
    {
      return 0.0;
    }
    return 1.0;
  }

  if(m_ucInPhase == kAttackPhase)
  {
    if(m_ulCurrent == m_ulAttackEnds)
    {
      m_ucInPhase = kDecayPhase;
      m_ulCurrent = 0;
    }
    else
    {
      fRet = ((double)m_ulCurrent) * m_fMsecsBetweenSamples * 1.0f / m_fAttack;
      m_ulCurrent++;
    }
  }
  if(m_ucInPhase == kDecayPhase)
  {
    if(m_ulCurrent == m_ulDecayEnds)
    {
      m_ucInPhase = kSustainPhase;
      m_ulCurrent = 0;
    }
    else
    {
      fRet = 1.0f - ((double)m_ulCurrent) * m_fMsecsBetweenSamples * (1.0f - m_fSustain) / m_fDecay;
      m_ulCurrent++;
    }
  }
  if(m_ucInPhase == kSustainPhase)
  {
    fRet = m_fSustain;
  }
  if(m_ucInPhase == kReleasePhase)
  {
    if(m_ulCurrent == m_ulReleaseEnds)
    {
      m_ucInPhase = kIdle;
      m_ulCurrent = 0;
    }
    else
    {
      fRet = m_fSustain - ((double)m_ulCurrent) * m_fMsecsBetweenSamples * m_fSustain / m_fRelease;
      m_ulCurrent++;
    }
  }
  if(m_ucInPhase == kIdle)
  {
    fRet = 0.0;
  }
  return fRet;
}
