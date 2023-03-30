#include "ProphetW.h"
#include "IPlug_include_in_plug_src.h"

#if IPLUG_EDITOR
#include "IControls.h"
#endif

ProphetW::ProphetW(const InstanceInfo& info)
  : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kParamAttack)->InitDouble("Attack", 0., 0., 1000.0, 0.1, "msec");
  GetParam(kParamDecay)->InitDouble("Decay", 1000., 0., 3000.0, 0.1, "msec");
  GetParam(kParamSustain)->InitDouble("Sustain", 0., 0., 1.0, 0.1, "%");
  GetParam(kParamRelease)->InitDouble("Release", 0., 0., 3000.0, 1.0, "msec");

  GetParam(kParamOsc1)->InitDouble("Osc1", 0., 0., 3.0, 1.0, "wave");
  GetParam(kParamOsc2)->InitDouble("Osc2", 0., 0., 3.0, 1.0, "wave");
  GetParam(kParamOsc3)->InitDouble("Osc3", 0., 0., 3.0, 1.0, "wave");
  GetParam(kParamOsc4)->InitDouble("Osc4", 0., 0., 3.0, 1.0, "wave");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS);
  };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    const IBitmap knobLittleBitmap = pGraphics->LoadBitmap(PNGENVELOP_FN, 128);
    const IBitmap knobOscBitmap = pGraphics->LoadBitmap(PNGOSC_FN, 3);

    const IRECT bounds = pGraphics->GetBounds();
    const IRECT innerBounds = bounds.GetPadded(-200.f);
    const IRECT sliderBoundsAttack = innerBounds.GetFromLeft(200).GetMidVPadded(100);
    const IRECT sliderBoundsDecay = innerBounds.GetFromLeft(300).GetMidVPadded(100);
    const IRECT sliderBoundsRelease = innerBounds.GetFromLeft(400).GetMidVPadded(100);
    const IRECT sliderBoundsSustain = innerBounds.GetFromLeft(500).GetMidVPadded(100);
    const IRECT versionBounds = innerBounds.GetFromTRHC(300, 20);
    const IRECT titleBounds = innerBounds.GetCentredInside(200, 50);


    pGraphics->SetLayoutOnResize(true);
    pGraphics->AttachCornerResizer(EUIResizerMode::Size, true);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    pGraphics->AttachPanelBackground(COLOR_LIGHT_GRAY);

    pGraphics->AttachControl(new IBKnobControl(510, 30, knobLittleBitmap, kParamAttack));
    pGraphics->AttachControl(new IBKnobControl(610, 30, knobLittleBitmap, kParamDecay));
    pGraphics->AttachControl(new IBKnobControl(710, 30, knobLittleBitmap, kParamSustain));
    pGraphics->AttachControl(new IBKnobControl(810, 30, knobLittleBitmap, kParamRelease));


    pGraphics->AttachControl(new IBKnobControl(110, 30, knobOscBitmap, kParamOsc1));
    pGraphics->AttachControl(new IBKnobControl(210, 30, knobOscBitmap, kParamOsc2));
    pGraphics->AttachControl(new IBKnobControl(310, 30, knobOscBitmap, kParamOsc3));
    pGraphics->AttachControl(new IBKnobControl(410, 30, knobOscBitmap, kParamOsc4));







    pGraphics->AttachControl(new ITextControl(titleBounds, "ProphetW", IText(30)), kCtrlTagTitle);
    WDL_String buildInfoStr;
    GetBuildInfoStr(buildInfoStr, __DATE__, __TIME__);
    pGraphics->AttachControl(new ITextControl(versionBounds, buildInfoStr.Get(), DEFAULT_TEXT.WithAlign(EAlign::Far)), kCtrlTagVersionNumber);

#if 1 // Show a keyboard or not
    //
    // Keyboard begins
    //
    const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
    const IRECT lfoPanel = b.GetFromLeft(300.f).GetFromTop(200.f);
    IRECT keyboardBounds = b.GetFromBottom(300);
    IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);

    pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
    pGraphics->AttachControl(new IVButtonControl(keyboardBounds.GetFromTRHC(200, 30).GetTranslated(0, -30), SplashClickActionFunc,
      "Show/Hide Keyboard", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE).WithLabelText({ 15.f, EVAlign::Middle })))->SetAnimationEndActionFunction(
        [pGraphics](IControl* pCaller) {
          static bool hide = false;
          pGraphics->GetControlWithTag(kCtrlTagKeyboard)->Hide(hide = !hide);
          pGraphics->Resize(PLUG_WIDTH, hide ? PLUG_HEIGHT / 2 : PLUG_HEIGHT, pGraphics->GetDrawScale());
        });
    //
    // Keyboard ends
    //
#endif // Show a keyboard or not



  };
#endif
}

#if IPLUG_EDITOR
void ProphetW::OnParentWindowResize(int width, int height)
{
  if (GetUI())
    GetUI()->Resize(width, height, 1.f, false);
}

void ProphetW::OnReset()
{
  mSynth.setSampleRate(static_cast<long>(GetSampleRate()));
  mSynth.setWaveform(1, Oscilator::kSawTooth);
  mSynth.setWaveform(2, Oscilator::kSawTooth);
  mSynth.setWaveform(3, Oscilator::kSawTooth);
  mSynth.setWaveform(4, Oscilator::kSawTooth);
  //  mSynth.NoteOn(40);
}
#endif


#if IPLUG_DSP
void ProphetW::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  // Channel declaration.
  PLUG_SAMPLE_DST* out01 = outputs[0];  PLUG_SAMPLE_DST* out02 = outputs[1];

  for (int s = 0; s < nFrames; s++)
  {
    while (!mMidiQueue.Empty())
    {
      IMidiMsg msg = mMidiQueue.Peek();
      if (msg.StatusMsg() == IMidiMsg::kNoteOn)
      {
        mSynth.NoteOn(msg.NoteNumber());
      }
      else if (msg.StatusMsg() == IMidiMsg::kNoteOff)
      {
        mSynth.NoteOff(msg.NoteNumber());
      }
      mMidiQueue.Remove();
    }
    *out01++ = mSynth.getLeft();
    *out02++ = mSynth.getRight();
  }
}

void ProphetW::ProcessMidiMsg(const IMidiMsg& msg)
{
  TRACE;
  mMidiQueue.Add(msg); // Take care of MIDI events in ProcessBlock()
}
void ProphetW::OnParamChange(int paramIdx)
{
  double value = GetParam(paramIdx)->Value();

  switch (paramIdx)
  {
  case kParamAttack:
    mSynth.setEnvelope(Envelope::kAttack, value);
    break;
  case kParamDecay:
    mSynth.setEnvelope(Envelope::kDecay, value);
    break;
  case kParamSustain:
    mSynth.setEnvelope(Envelope::kSustain, value);
    break;
  case kParamRelease:
    mSynth.setEnvelope(Envelope::kRelease, value);
    break;
  case kParamOsc1:
    mSynth.setWaveform(1, value);
    break;
  case kParamOsc2:
    mSynth.setWaveform(2, value);
    break;
  case kParamOsc3:
    mSynth.setWaveform(3, value);
    break;
  case kParamOsc4:
    mSynth.setWaveform(4, value);
    break;
  }
}

#endif
