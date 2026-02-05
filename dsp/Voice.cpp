#include "Voice.h"
#ifdef _WIN32
#include <windows.h>  // for OutputDebugString
#endif
#include <string>
#include <cmath> // For std::pow
#include <algorithm> // For std::clamp

#ifdef OLA_LOG_TO_FILE
#include <filesystem>
#include <fstream>
extern std::ofstream gLogFile;
#endif

Voice::Voice() : m_ucNote(0)
{
  for (int i = 0; i < 16; i++)
  {
    m_osc[i] = Oscilator();
    if (i % 4 == 0)
    {
      m_osc[i].setWaveform(Oscilator::kSawTooth);
    }
    else if (i % 4 == 1)
    {
      m_osc[i].setWaveform(Oscilator::kSquare);
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
  mVolumeEnvelope = Envelope();
  mFilterEnvelope = Envelope();

  mVolume = 1.0;  // Master volume

  // ------------- Build note to frequence table ------------------
  // Convert note to frequence.
  double k = 1.059463094359;     // 12th root of 2
  double a = 6.875;              // a
  a *= k;                        // b
  a *= k;                        // bb
  a *= k;                        // c, frequency of midi note 0
  for (int i = 0; i < 140; i++)  // 140 midi notes
  {
    m_note2freq[i] = (double)a;
    a *= k;
  }
}

void
Voice::setOscVol(int oscNr, double vol)
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

void
Voice::setOscFreq(int oscNr, double freq)
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

void
Voice::setOscFine(int oscNr, double freq)
{
  switch (oscNr)
  {
    case 0:
      mOsc1Fine = freq;
      //    OutputDebugStringA(std::string("mOsc1Fine = " + std::to_string(mOsc1Fine) + "\n").c_str());
      break;
    case 1: mOsc2Fine = freq; break;
    case 2: mOsc3Fine = freq; break;
    case 3: mOsc4Fine = freq; break;
    default: throw "Unknown oscilator";
  }
}

void
Voice::setOscPulseWidth(int oscNr, double pulseWidth)
{
  switch (oscNr)
  {
    case 0: mOsc1PulseWidth = pulseWidth; break;
    case 1: mOsc2PulseWidth = pulseWidth; break;
    case 2: mOsc3PulseWidth = pulseWidth; break;
    case 3: mOsc4PulseWidth = pulseWidth; break;
    default: throw "Unknown oscilator";
  }
}

void
Voice::setVolumeEnvelope(Envelope::type parameter, double value)
{
  switch (parameter)
  {
    case Envelope::kAttack: mVolumeEnvelope.setAttack(value); break;
    case Envelope::kDecay: mVolumeEnvelope.setDecay(value); break;
    case Envelope::kSustain: mVolumeEnvelope.setSustain(value); break;
    case Envelope::kRelease: mVolumeEnvelope.setRelease(value); break;
    default: throw "Unknown volume envelope";
  }
}

void
Voice::setFilterEnvelope(Envelope::type parameter, double value)
{
  switch (parameter)
  {
    case Envelope::kAttack: mFilterEnvelope.setAttack(value); break;
    case Envelope::kDecay: mFilterEnvelope.setDecay(value); break;
    case Envelope::kSustain: mFilterEnvelope.setSustain(value); break;
    case Envelope::kRelease: mFilterEnvelope.setRelease(value); break;
    case Envelope::kAmount: mFilterEnvelope.setAmount(value); break;
    default: throw "Unknown envelope";
  }
}

void
Voice::setSampleRate(long sampleRate)
{
  mSampleRate = sampleRate;
  for (int i = 0; i < 16; i++)
  {
    m_osc[i].setSampleRate(sampleRate);
  }
  mFilter.setSampleRate(sampleRate);
  mVolumeEnvelope.setSampleRate(sampleRate);
  mFilterEnvelope.setSampleRate(sampleRate);
}

void
Voice::setCutOff(double cutOff)
{
  mCutOff = cutOff;
  mFilter.setCutoff(mCutOff);
}

void
Voice::setResonance(double res)
{
  mResonance = res;
  mFilter.setResonance(mResonance);
}

void
Voice::NoteOn(unsigned char ucNote)
{
  m_ucNote = ucNote;

  for (int i = 0; i < 16; i++)
  {
    m_osc[i].noteOn();
  }

  mVolumeEnvelope.restart();
  mFilterEnvelope.restart();
}

void
Voice::NoteOff(unsigned char ucNote)
{
  //for (int i = 0; i < 16; i++)
  //{
  //  m_osc[i].noteOff();
  //}
  mVolumeEnvelope.beginReleasePhase();
  mFilterEnvelope.beginReleasePhase();
}

static double m_fCurrentEnvelopeVal;

double
Voice::getMono()
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

  double osc1Fine = mOsc1Fine < 0 ?
                        mOsc1Fine * (m_note2freq[osc1Note] - m_note2freq[osc1Note - 1]) :
                        mOsc1Fine * (m_note2freq[osc1Note + 1] - m_note2freq[osc1Note]);
  double osc2Fine = mOsc2Fine < 0 ?
                        mOsc2Fine * (m_note2freq[osc2Note] - m_note2freq[osc2Note - 1]) :
                        mOsc2Fine * (m_note2freq[osc2Note + 1] - m_note2freq[osc2Note]);
  double osc3Fine = mOsc3Fine < 0 ?
                        mOsc3Fine * (m_note2freq[osc3Note] - m_note2freq[osc3Note - 1]) :
                        mOsc3Fine * (m_note2freq[osc3Note + 1] - m_note2freq[osc3Note]);
  double osc4Fine = mOsc4Fine < 0 ?
                        mOsc4Fine * (m_note2freq[osc4Note] - m_note2freq[osc4Note - 1]) :
                        mOsc4Fine * (m_note2freq[osc4Note + 1] - m_note2freq[osc4Note]);

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

  m_osc[0].setPulseWidth(mOsc1PulseWidth);
  m_osc[1].setPulseWidth(mOsc1PulseWidth);
  m_osc[2].setPulseWidth(mOsc1PulseWidth);
  m_osc[3].setPulseWidth(mOsc1PulseWidth);

  m_osc[4].setPulseWidth(mOsc2PulseWidth);
  m_osc[5].setPulseWidth(mOsc2PulseWidth);
  m_osc[6].setPulseWidth(mOsc2PulseWidth);
  m_osc[7].setPulseWidth(mOsc2PulseWidth);

  m_osc[8].setPulseWidth(mOsc3PulseWidth);
  m_osc[9].setPulseWidth(mOsc3PulseWidth);
  m_osc[10].setPulseWidth(mOsc3PulseWidth);
  m_osc[11].setPulseWidth(mOsc3PulseWidth);

  m_osc[12].setPulseWidth(mOsc4PulseWidth);
  m_osc[13].setPulseWidth(mOsc4PulseWidth);
  m_osc[14].setPulseWidth(mOsc4PulseWidth);
  m_osc[15].setPulseWidth(mOsc4PulseWidth);

  double osc1divider = (m_osc[0].getIsOn() ? 1 : 0) + (m_osc[1].getIsOn() ? 1 : 0) +
                       (m_osc[2].getIsOn() ? 1 : 0) + (m_osc[3].getIsOn() ? 1 : 0);
  double osc2divider = (m_osc[4].getIsOn() ? 1 : 0) + (m_osc[5].getIsOn() ? 1 : 0) +
                       (m_osc[6].getIsOn() ? 1 : 0) + (m_osc[7].getIsOn() ? 1 : 0);
  double osc3divider = (m_osc[8].getIsOn() ? 1 : 0) + (m_osc[9].getIsOn() ? 1 : 0) +
                       (m_osc[10].getIsOn() ? 1 : 0) + (m_osc[11].getIsOn() ? 1 : 0);
  double osc4divider = (m_osc[12].getIsOn() ? 1 : 0) + (m_osc[13].getIsOn() ? 1 : 0) +
                       (m_osc[14].getIsOn() ? 1 : 0) + (m_osc[15].getIsOn() ? 1 : 0);
  osc1divider = osc1divider == 0 ? 1 : osc1divider;
  osc2divider = osc2divider == 0 ? 1 : osc2divider;
  osc3divider = osc3divider == 0 ? 1 : osc3divider;
  osc4divider = osc4divider == 0 ? 1 : osc4divider;

  double osc1 = (m_osc[0].get() + m_osc[1].get() + m_osc[2].get() + m_osc[3].get()) / osc1divider;
  double osc2 = (m_osc[4].get() + m_osc[5].get() + m_osc[6].get() + m_osc[7].get()) / osc2divider;
  double osc3 = (m_osc[8].get() + m_osc[9].get() + m_osc[10].get() + m_osc[11].get()) / osc3divider;
  double osc4 =
      (m_osc[12].get() + m_osc[13].get() + m_osc[14].get() + m_osc[15].get()) / osc4divider;

  double allOscilators = ((osc1 * mOsc1Vol + osc2 * mOsc2Vol + osc3 * mOsc3Vol + osc4 * mOsc4Vol) / 4.0);

  double filtered;
  if (mFilterOn)
  {
    double filterEnvelopeValue = mFilterEnvelope.get();

    double baseCutoff = mCutOff;
    double minCutoffEnv = 20.0;     // Lowest frequency the envelope can bring the cutoff
    double maxCutoffEnv = 20000.0;  // Highest frequency the envelope can bring the cutoff

    double filterEnvAmount = mFilterEnvelope.getAmount();

    double envelopeModulatedFreq =
        minCutoffEnv * std::pow((maxCutoffEnv / minCutoffEnv), filterEnvelopeValue);

    double finalCutoff =
        (baseCutoff * (1.0 - filterEnvAmount)) + (envelopeModulatedFreq * filterEnvAmount);

    finalCutoff = std::clamp(finalCutoff,
                             20.0,
                             mSampleRate / 2.0 - 100.0);  // Use filter's max safe cutoff

#ifdef OLA_LOG_TO_FILE
    gLogFile << "Before filter: " << allOscilators << " \n";
    gLogFile.flush();
#endif
    mFilter.setCutoff(finalCutoff);
    //    mFilter.setResonance(0.5);
    filtered = mFilter.process(allOscilators);
#ifdef OLA_LOG_TO_FILE
    gLogFile << "After filter: " << filtered << " \n";
    gLogFile.flush();
#endif
  }
  else
  {
    filtered = allOscilators;
  }

  double dVolumeEnvelopeValue = mVolumeEnvelope.get();

  return filtered * dVolumeEnvelopeValue * mVolume;
}

double
Voice::getLeft()
{
  return getMono();
}

double
Voice::getRight()
{
  return getMono();
}
