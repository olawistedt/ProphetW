#include "Synth.h"

Synth::Synth()
{
  m_osc1 = Oscilator();
  m_osc2 = Oscilator();
  m_osc3 = Oscilator();
  m_osc1.setWaveform(Oscilator::kSquare);
  m_osc2.setWaveform(Oscilator::kSawTooth);
  m_osc3.setWaveform(Oscilator::kSine);
  m_envelope = Envelope();

  m_fOsc1Vol = 0.5f;
  m_fOsc2Vol = 0.5f;
  m_fOsc3Vol = 0.5f;
  m_fVolume = 0.5f;

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
  m_envelope.setSampleRate(sampleRate);
}

void Synth::NoteOn(unsigned char ucNote)
{
  m_osc1.setFreq(m_note2freq[ucNote]);
  m_osc2.setFreq(m_note2freq[ucNote]);
  m_osc3.setFreq(m_note2freq[ucNote]);
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
  return ((m_osc1.get() * m_fOsc1Vol +
    m_osc2.get() * m_fOsc2Vol +
    m_osc3.get() * m_fOsc3Vol) / 3.0) * m_fCurrentEnvelopeVal * m_fVolume;
}

double Synth::getRight()
{
  m_fCurrentEnvelopeVal = m_envelope.get();
  return ((m_osc1.get() * m_fOsc1Vol +
    m_osc2.get() * m_fOsc2Vol +
    m_osc3.get() * m_fOsc3Vol) / 3.0) * m_fCurrentEnvelopeVal * m_fVolume;
}
