#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "dsp/Voice.h"

const int kNumPresets = 1;
#define NUMBER_OF_VOICES 10


enum EParams
{
  kMainVolume = 0,
  kParamRelease,
  kParamAttack,
  kParamDecay,
  kParamSustain,
  kParamOsc0,
  kParamOsc0Vol = kParamOsc0 + 16,
  kParamOsc0Freq = kParamOsc0Vol + 4,
  kParamOsc0Fine = kParamOsc0Freq + 4,
  kParamOsc0PulseWidth = kParamOsc0Fine + 4,
  kParamFilterCutoff = kParamOsc0PulseWidth + 4,
  kParamFilterResonance,
  kNumParams
};

enum ECtrlTags
{
  kCtrlTagVersionNumber = 0,
  kCtrlTagSliderAttack,
  kCtrlTagSliderDecay,
  kCtrlTagSliderSustain,
  kCtrlTagSliderRelease,
  kCtrlTagTitle,
  kCtrlTagKeyboard
};


using namespace iplug;
using namespace igraphics;

class ProphetW final : public Plugin
{
public:
  ProphetW(const InstanceInfo &info);

#if IPLUG_EDITOR
  bool OnHostRequestingSupportedViewConfiguration(int width, int height) override { return true; }
#endif

#if IPLUG_DSP  // http://bit.ly/2S64BDd
  void ProcessBlock(sample **inputs, sample **outputs, int nFrames) override;
  void OnReset() override;
  void ProcessMidiMsg(const IMidiMsg &msg) override;
  //  void OnParamChange(int paramIdx) override;
  void OnParamChangeUI(int paramIdx, EParamSource source = kUnknown) override;

  Voice mVoice[NUMBER_OF_VOICES];

protected:
  IMidiQueue mMidiQueue;
#endif
};
