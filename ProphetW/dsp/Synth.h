#ifndef __SYNTH_H__
#define __SYNTH_H__

#include "Oscilator.h"
#include "Envelope.h"

class Synth
{
public:
  Synth();
  double getLeft();
  double getRight();
  void setSampleRate(long sampleRate);
  void NoteOn (unsigned char ucNote);
  void NoteOff(unsigned char ucNote);

  Oscilator m_osc1;
  Oscilator m_osc2;
  Oscilator m_osc3;
  Envelope  m_envelope;
  double m_fOsc1Vol;
  double m_fOsc2Vol;
  double m_fOsc3Vol;
  double m_fVolume;

  double m_note2freq[128];

private:

};

#endif // __SYNTH_H__
