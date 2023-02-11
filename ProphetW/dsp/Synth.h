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

  double m_note2freq[128];

  void setWaveform(int oscNr, unsigned char waveform);
  void setEnvelope(Envelope::type parameter, double value);

private:
  Oscilator m_osc1;
  Oscilator m_osc2;
  Oscilator m_osc3;
  Oscilator m_osc4;
  Envelope  m_envelope;
  double m_fOsc1Vol;
  double m_fOsc2Vol;
  double m_fOsc3Vol;
  double m_fVolume;
};

#endif // __SYNTH_H__
