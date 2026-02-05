#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

class Envelope
{
public:
  Envelope();

  void setSampleRate(unsigned long ulNewSampleRate);
  double get();
  void restart();
  void beginReleasePhase();
  void setAttack(double dMsec);
  void setDecay(double dMsec);
  void setSustain(double dLevel);
  void setRelease(double dMsec);
  void setAmount(double dLevel);
  double getAmount();

  enum type
  {
    kAttack = 0,
    kDecay,
    kSustain,
    kRelease,
    kAmount,
    kNumEnvelopes
  };

private:
  enum
  {
    kIdle,
    kAttackPhase,
    kDecayPhase,
    kSustainPhase,
    kReleasePhase,
  };

  double m_fAttack;
  double m_fDecay;
  double m_fSustain;
  double m_fRelease;
  double m_fAmount;
  unsigned long m_ulSampleRate;
  double m_fMsecsBetweenSamples;
  double m_dReleaseStartLevel;
  unsigned long m_ulCurrent;
  unsigned long m_ulAttackEnds;
  unsigned long m_ulDecayEnds;
  unsigned long m_ulReleaseEnds;
  unsigned char m_ucInPhase;
};

#endif  // __ENVELOPE_H__
