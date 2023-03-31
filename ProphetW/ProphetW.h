#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "dsp/Synth.h"

const int kNumPresets = 1;

enum EParams
{
  kParamRelease = 0,
  kParamAttack,
  kParamDecay,
  kParamSustain,
  kParamOsc1,
  kParamOsc2,
  kParamOsc3,
  kParamOsc4,
  kParamOsc1Vol,
  kParamOsc2Vol,
  kParamOsc3Vol,
  kParamOsc4Vol,
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
  ProphetW(const InstanceInfo& info);

#if IPLUG_EDITOR
  void OnParentWindowResize(int width, int height) override;
  bool OnHostRequestingSupportedViewConfiguration(int width, int height) override { return true; }
#endif
  
#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void ProcessMidiMsg(const IMidiMsg& msg) override;
//  void OnParamChange(int paramIdx) override;
  void OnParamChangeUI(int paramIdx, EParamSource source = kUnknown) override;

  Synth mSynth;
protected:
  IMidiQueue mMidiQueue;
#endif

};
