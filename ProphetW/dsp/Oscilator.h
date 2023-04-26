#ifndef __OSCILATOR_H__
#define __OSCILATOR_H__

//------------------------------------------------------------------------------------------
// Oscilator
//------------------------------------------------------------------------------------------
class Oscilator
{
public:
  Oscilator();
  //  ~Oscilator     ();
  double         get();
  void           setSampleRate(unsigned long);
  void           setFreq(double);
  void           noteOn();
  void           setWaveform(unsigned char ucWaveform);
  void           setPulseWidth(double fPulseWidthInPercent);
  void           setOsc(int oscNr, int value);
  void           setVolume(double volume); // The  volume of the combinded oscilators.
  void           setIsOn(bool isOn) { m_bIsOn = isOn; }
  bool           getIsOn() { return m_bIsOn; }
//  unsigned short period(double* values);
#ifdef __LOG_FILE__
  void           period2logfile();
#endif
  enum oscType
  {
    kWaveBufSize = 4096,

    // Waveform tags
    kSquare = 0,
    kSawTooth,
    kTriangle,
    kSine,
    kNumWaves
  };

private:
  double         getSquare();
  double         getSawTooth();
  double         getTriangle();
  double         getSine();
  unsigned short m_usPeriodLength;        // Period legth in bits.
  unsigned short m_usPulseWidth;          // Pulse width in bits.
  unsigned long  m_ulSampleRate;
  double         m_dFreq;
  unsigned char  m_ucWaveform;
  double         m_dVolume;
  unsigned short m_usCurrent;             // x position in sample curve.
  double         m_dPulseWidthInPercent;
  bool           m_bIsOn;
};

#endif // __OSCILATOR_H__


