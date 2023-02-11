#ifndef __OSCILATOR_H__
#define __OSCILATOR_H__

//------------------------------------------------------------------------------------------
// Oscilator
//------------------------------------------------------------------------------------------
class Oscilator
{
public:
  Oscilator      ();
//  ~Oscilator     ();
  double         get();
  void           setSampleRate(unsigned long);
  void           setFreq(double);
  void           setWaveform(unsigned char ucWaveform);
  void           setPulseWidth(double fPulseWidthInPercent);
  void           setOsc(int oscNr, int value);
  unsigned short period(double* values);
#ifdef __LOG_FILE__
  void           period2logfile();
#endif
  enum oscType
  {
    kWaveBufSize = 4096,
      
    // Waveform tags
    kSquare = 0,
    kSawTooth,
    kSine,
    kNumWaves
  };

private:
  double         getSquare();
  double         getSawTooth();
  double         getSine();
  unsigned short m_usPeriodLength;        // Period legth in bits.
  unsigned short m_usPulseWidth;          // Pulse width in bits.
  unsigned long  m_ulSampleRate;
  double         m_fFreq;
  unsigned char  m_ucWaveform;
  unsigned short m_usCurrent;             // x position in sample curve.
  double         m_fPulseWidthInPercent;
};

#endif // __OSCILATOR_H__


