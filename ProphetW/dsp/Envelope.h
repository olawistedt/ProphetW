#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__

class Envelope
{
 public:
  Envelope();

  void  setSampleRate(unsigned long ulNewSampleRate);
  double get               (); 
  void  restart           ();
  void  beginReleasePhase ();
  void  setAttack         (double fMsec);
  void  setDecay          (double fMsec);
  void  setSustain        (double fLevel);
  void  setRelease        (double fMsec);
 private:
  enum
  {
    kIdle,
    kAttackPhase,
    kDecayPhase,
    kSustainPhase,
    kReleasePhase,
  };

  double        m_fAttack;
  double        m_fDecay;
  double        m_fSustain;
  double        m_fRelease;
  unsigned long m_ulSampleRate;
  double        m_fMsecsBetweenSamples;
  unsigned long m_ulCurrent;
  unsigned long m_ulAttackEnds;
  unsigned long m_ulDecayEnds;
  unsigned long m_ulReleaseEnds;
  unsigned char m_ucInPhase;
};

#endif // __ENVELOPE_H__
