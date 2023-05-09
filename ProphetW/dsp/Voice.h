#ifndef __VOICE_H__
#define __VOICE_H__

#include "Oscilator.h"
#include "Envelope.h"

class Voice
{
public:
  Voice();
  double getMono();
  double getLeft();
  double getRight();
  void setSampleRate(long sampleRate);
  void NoteOn(unsigned char ucNote);
  void NoteOff(unsigned char ucNote);
  void setOscVol(int oscNr, double vol);
  void setOscFreq(int oscNr, double freq);
  void setOscFine(int oscNr, double freq);
  void setOscPulseWidth(int oscNr, double pulseWidth);
  void setEnvelope(Envelope::type parameter, double value);
  void setCutOff(double value);
  void setResonance(double value);
  void setMasterVolume(double volume) { mVolume = volume; }

  double m_note2freq[140];
  Oscilator m_osc[16];

private:
  unsigned char m_ucNote;
  double mOsc1Vol;
  double mOsc2Vol;
  double mOsc3Vol;
  double mOsc4Vol;
  int mOsc1Freq;
  int mOsc2Freq;
  int mOsc3Freq;
  int mOsc4Freq;
  double mOsc1Fine;
  double mOsc2Fine;
  double mOsc3Fine;
  double mOsc4Fine;
  double mOsc1PulseWidth;
  double mOsc2PulseWidth;
  double mOsc3PulseWidth;
  double mOsc4PulseWidth;
  Envelope m_envelope;
  double mVolume;
  bool mIsOn;
};

#endif  // __VOICE_H__
