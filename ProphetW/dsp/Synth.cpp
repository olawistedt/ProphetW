#include "Synth.h"

Synth::Synth()
{
  m_osc1 = Oscilator();
  m_osc2 = Oscilator();
  m_osc3 = Oscilator();
  m_osc4 = Oscilator();
  m_osc1.setWaveform(Oscilator::kSquare);
  m_osc2.setWaveform(Oscilator::kSawTooth);
  m_osc3.setWaveform(Oscilator::kSine);
  m_osc4.setWaveform(Oscilator::kSine);
  m_envelope = Envelope();

  mOsc1Vol = 0.5f;
  mOsc2Vol = 0.0f;
  mOsc3Vol = 0.0f;
  mOsc4Vol = 0.0f;
  mVolume = 0.5f;

  // ------------- Build note to frequence table ------------------
  // Convert note to frequence.
  double k = 1.059463094359;  // 12th root of 2
  double a = 6.875;           // a
  a *= k;                     // b
  a *= k;                     // bb
  a *= k;                     // c, frequency of midi note 0
  for (int i = 0; i < 128; i++) // 128 midi notes
  {
    m_note2freq[i] = (double)a;
    a *= k;
  }
}

void Synth::setWaveform(int oscNr, unsigned char waveform)
{
  switch (oscNr)
  {
  case 1: m_osc1.setWaveform(waveform); break;
  case 2: m_osc2.setWaveform(waveform); break;
  case 3: m_osc3.setWaveform(waveform); break;
  case 4: m_osc4.setWaveform(waveform); break;
  default: throw "Unknown waveform";
  }
}

void Synth::setOscVol(int oscNr, double vol)
{
  switch (oscNr)
  {
  case 1: mOsc1Vol = vol; break;
  case 2: mOsc2Vol = vol; break;
  case 3: mOsc3Vol = vol; break;
  case 4: mOsc4Vol = vol; break;
  default: throw "Unknown oscilator";
  }
}

void Synth::setEnvelope(Envelope::type parameter, double value)
{
  switch (parameter)
  {
  case Envelope::kAttack:  m_envelope.setAttack(value); break;
  case Envelope::kDecay:   m_envelope.setDecay(value); break;
  case Envelope::kSustain: m_envelope.setSustain(value); break;
  case Envelope::kRelease: m_envelope.setRelease(value); break;
  default: throw "Unknown envelope";
  }
}

void Synth::setSampleRate(long sampleRate)
{
  m_osc1.setSampleRate(sampleRate);
  m_osc2.setSampleRate(sampleRate);
  m_osc3.setSampleRate(sampleRate);
  m_osc4.setSampleRate(sampleRate);
  m_envelope.setSampleRate(sampleRate);
}

void Synth::NoteOn(unsigned char ucNote)
{
  m_osc1.setFreq(m_note2freq[ucNote]);
  m_osc2.setFreq(m_note2freq[ucNote]);
  m_osc3.setFreq(m_note2freq[ucNote]);
  m_osc4.setFreq(m_note2freq[ucNote]);
  m_envelope.restart();
}

void Synth::NoteOff(unsigned char ucNote)
{
  m_envelope.beginReleasePhase();
}

static double m_fCurrentEnvelopeVal;

double Synth::getLeft()
{
  m_fCurrentEnvelopeVal = m_envelope.get();
  return ((m_osc1.get() * mOsc1Vol +
    m_osc2.get() * mOsc2Vol +
    m_osc3.get() * mOsc3Vol +
    m_osc4.get() * mOsc4Vol) / 4.0) * m_fCurrentEnvelopeVal * mVolume;
}

double Synth::getRight()
{
  m_fCurrentEnvelopeVal = m_envelope.get();
  return ((m_osc1.get() * mOsc1Vol +
    m_osc2.get() * mOsc2Vol +
    m_osc3.get() * mOsc3Vol +
    m_osc4.get() * mOsc4Vol) / 4.0) * m_fCurrentEnvelopeVal * mVolume;
}
