#include "Synth.h"
#ifdef _WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <string>

Synth::Synth()
{
  for (int i = 0; i < 16; i++)
  {
    m_osc[i] = Oscilator();
    if (i % 4 == 0)
    {
      m_osc[i].setWaveform(Oscilator::kSquare);
    }
    else if (i % 4 == 1)
    {
      m_osc[i].setWaveform(Oscilator::kSawTooth);
    }
    else if (i % 4 == 2)
    {
      m_osc[i].setWaveform(Oscilator::kTriangle);
    }
    else if (i % 4 == 3)
    {
      m_osc[i].setWaveform(Oscilator::kSine);
    }
  }
  m_envelope = Envelope();

  mVolume = 1.0; // Master volume

  // ------------- Build note to frequence table ------------------
  // Convert note to frequence.
  double k = 1.059463094359;  // 12th root of 2
  double a = 6.875;           // a
  a *= k;                     // b
  a *= k;                     // bb
  a *= k;                     // c, frequency of midi note 0
  for (int i = 0; i < 140; i++) // 140 midi notes
  {
    m_note2freq[i] = (double)a;
    a *= k;
  }
}

void Synth::setOscVol(int oscNr, double vol)
{
  switch (oscNr)
  {
  case 0: mOsc1Vol = vol; break;
  case 1: mOsc2Vol = vol; break;
  case 2: mOsc3Vol = vol; break;
  case 3: mOsc4Vol = vol; break;
  default: throw "Unknown oscilator";
  }
}

void Synth::setOscFreq(int oscNr, double freq)
{
  switch (oscNr)
  {
  case 0: mOsc1Freq = static_cast<int>(freq); break;
  case 1: mOsc2Freq = static_cast<int>(freq); break;
  case 2: mOsc3Freq = static_cast<int>(freq); break;
  case 3: mOsc4Freq = static_cast<int>(freq); break;
  default: throw "Unknown oscilator";
  }
}

void Synth::setOscFine(int oscNr, double freq)
{
  switch (oscNr)
  {
  case 0: mOsc1Fine = freq;
//    OutputDebugStringA(std::string("mOsc1Fine = " + std::to_string(mOsc1Fine) + "\n").c_str());
    break;
  case 1: mOsc2Fine = freq; break;
  case 2: mOsc3Fine = freq; break;
  case 3: mOsc4Fine = freq; break;
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
  for (int i = 0; i < 16; i++)
  {
    m_osc[i].setSampleRate(sampleRate);
  }

  m_envelope.setSampleRate(sampleRate);
}

void Synth::NoteOn(unsigned char ucNote)
{
  m_ucNote = ucNote;

  for (int i = 0; i < 16; i++)
  {
    m_osc[i].noteOn();
  }

  m_envelope.restart();
}

void Synth::NoteOff(unsigned char ucNote)
{
  m_envelope.beginReleasePhase();
}

static double m_fCurrentEnvelopeVal;

double Synth::getMono()
{
  int osc1Note = m_ucNote + mOsc1Freq;
  int osc2Note = m_ucNote + mOsc2Freq;
  int osc3Note = m_ucNote + mOsc3Freq;
  int osc4Note = m_ucNote + mOsc4Freq;

  if (osc1Note < 1 || osc1Note >= 138)
  {
    osc1Note = m_ucNote;
  }
  if (osc2Note < 1 || osc2Note >= 138)
  {
    osc2Note = m_ucNote;
  }
  if (osc3Note < 1 || osc3Note >= 138)
  {
    osc3Note = m_ucNote;
  }
  if (osc4Note < 1 || osc4Note >= 138)
  {
    osc4Note = m_ucNote;
  }

  double osc1Fine = mOsc1Fine < 0 ? mOsc1Fine * (m_note2freq[osc1Note] - m_note2freq[osc1Note - 1]) : mOsc1Fine * (m_note2freq[osc1Note + 1] - m_note2freq[osc1Note]);
  double osc2Fine = mOsc2Fine < 0 ? mOsc2Fine * (m_note2freq[osc2Note] - m_note2freq[osc2Note - 1]) : mOsc2Fine * (m_note2freq[osc2Note + 1] - m_note2freq[osc2Note]);
  double osc3Fine = mOsc3Fine < 0 ? mOsc3Fine * (m_note2freq[osc3Note] - m_note2freq[osc3Note - 1]) : mOsc3Fine * (m_note2freq[osc3Note + 1] - m_note2freq[osc3Note]);
  double osc4Fine = mOsc4Fine < 0 ? mOsc4Fine * (m_note2freq[osc4Note] - m_note2freq[osc4Note - 1]) : mOsc4Fine * (m_note2freq[osc4Note + 1] - m_note2freq[osc4Note]);

  m_osc[0].setFreq(m_note2freq[osc1Note] + osc1Fine);
  m_osc[1].setFreq(m_note2freq[osc1Note] + osc1Fine);
  m_osc[2].setFreq(m_note2freq[osc1Note] + osc1Fine);
  m_osc[3].setFreq(m_note2freq[osc1Note] + osc1Fine);

  m_osc[4].setFreq(m_note2freq[osc2Note] + osc2Fine);
  m_osc[5].setFreq(m_note2freq[osc2Note] + osc2Fine);
  m_osc[6].setFreq(m_note2freq[osc2Note] + osc2Fine);
  m_osc[7].setFreq(m_note2freq[osc2Note] + osc2Fine);

  m_osc[8].setFreq(m_note2freq[osc3Note] + osc3Fine);
  m_osc[9].setFreq(m_note2freq[osc3Note] + osc3Fine);
  m_osc[10].setFreq(m_note2freq[osc3Note] + osc3Fine);
  m_osc[11].setFreq(m_note2freq[osc3Note] + osc3Fine);

  m_osc[12].setFreq(m_note2freq[osc4Note] + osc4Fine);
  m_osc[13].setFreq(m_note2freq[osc4Note] + osc4Fine);
  m_osc[14].setFreq(m_note2freq[osc4Note] + osc4Fine);
  m_osc[15].setFreq(m_note2freq[osc4Note] + osc4Fine);


  double osc1 = m_osc[0].get() + m_osc[1].get() + m_osc[2].get() + m_osc[3].get();
  double osc2 = m_osc[4].get() + m_osc[5].get() + m_osc[6].get() + m_osc[7].get();
  double osc3 = m_osc[8].get() + m_osc[9].get() + m_osc[10].get() + m_osc[11].get();
  double osc4 = m_osc[12].get() + m_osc[13].get() + m_osc[14].get() + m_osc[15].get();

  double fCurrentEnvelopeVal = m_envelope.get();

  return ((osc1 * mOsc1Vol +
    osc2 * mOsc2Vol +
    osc3 * mOsc3Vol +
    osc4 * mOsc4Vol) / 4.0) * fCurrentEnvelopeVal * mVolume;
}

double Synth::getLeft()
{
  return getMono();
}

double Synth::getRight()
{
  return getMono();
}
