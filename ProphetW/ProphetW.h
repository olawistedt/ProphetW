#pragma once

#include "IPlug_include_in_plug_hdr.h"
#include "dsp/Synth.h"

const int kNumPresets = 1;

enum EParams
{
  kParamGain = 0,
  kNumParams
};

enum ECtrlTags
{
  kCtrlTagVersionNumber = 0,
  kCtrlTagSlider,
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
  Synth mSynth;
protected:
  IMidiQueue mMidiQueue;
#endif

};
