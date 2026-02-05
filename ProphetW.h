#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "dsp/Voice.h"

const int kNumPresets = 1;
const int kNumVoices = 10;

enum EParams
{
  kMainVolume = 0,
  kParamVolumeAttack,
  kParamVolumeDecay,
  kParamVolumeSustain,
  kParamVolumeRelease,
  kParamFilterAttack,
  kParamFilterDecay,
  kParamFilterSustain,
  kParamFilterRelease,
  kParamFilterEnvelopeAmount,
  kParamOsc0,
  kParamOsc0Vol = kParamOsc0 + 16,
  kParamOsc0Freq = kParamOsc0Vol + 4,
  kParamOsc0Fine = kParamOsc0Freq + 4,
  kParamOsc0PulseWidth = kParamOsc0Fine + 4,
  kParamFilterCutoff = kParamOsc0PulseWidth + 4,
  kParamFilterResonance,
  kParamFilterOnOff,
  kNumParams
};

enum ECtrlTags
{
  kCtrlTagVersionNumber = 0,
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

  Voice mVoice[kNumVoices];
  short mVoices[kNumVoices];

protected:
  IMidiQueue mMidiQueue;
  double mPlugUIScale;
#endif
};
