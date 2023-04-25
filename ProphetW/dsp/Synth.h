#ifndef __SYNTH_H__
#define __SYNTH_H__

#include "Oscilator.h"
#include "Envelope.h"

class Synth
{
public:
  Synth();
  double getMono();
  double getLeft();
  double getRight();
  void setSampleRate(long sampleRate);
  void NoteOn (unsigned char ucNote);
  void NoteOff(unsigned char ucNote);
  void setOscVol(int oscNr, double vol);
  void setEnvelope(Envelope::type parameter, double value);
  void setMasterVolume(double volume) { mVolume = volume; }

  double m_note2freq[128];
  Oscilator m_osc[16];

private:
  double mOsc1Vol;
  double mOsc2Vol;
  double mOsc3Vol;
  double mOsc4Vol;
  Envelope  m_envelope;
  double mVolume;
};

#endif // __SYNTH_H__
