#include "ProphetW.h"
#include "IPlug_include_in_plug_src.h"
#include <string>

#if IPLUG_EDITOR
#include "IControls.h"
#endif

ProphetW::ProphetW(const InstanceInfo &info) : Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  for (int i = 0; i < 10; i++)
  {
    mVoices[i] = -1;
  }

  GetParam(kParamAttack)->InitDouble("Attack", 0., 0., 1000.0, 0.1, "msec");
  GetParam(kParamDecay)->InitDouble("Decay", 3000., 0., 3000.0, 0.1, "msec");
  GetParam(kParamSustain)->InitDouble("Sustain", 0., 0., 1.0, 0.1, "%");
  GetParam(kParamRelease)->InitDouble("Release", 0., 0., 3000.0, 1.0, "msec");

  for (int i = 0; i < 16; i++)
  {
    GetParam(kParamOsc0 + i)
        ->InitBool(std::string("Osc" + std::to_string(i + 1)).c_str(),
                   i == kParamOsc0 ? true : false);
  }

  for (int i = 0; i < 4; i++)
  {
    GetParam(kParamOsc0Vol + i)
        ->InitDouble(std::string("Osc" + std::to_string(i + 1) + " Vol").c_str(),
                     1.0,
                     0.,
                     1.0,
                     0.1,
                     "db");
  }

  for (int i = 0; i < 4; i++)
  {
    GetParam(kParamOsc0Freq + i)
        ->InitDouble(std::string("Osc" + std::to_string(i + 1) + " Freq").c_str(),
                     0.0,
                     -24.0,
                     +24.0,
                     1.0,
                     "seminotes");
  }

  for (int i = 0; i < 4; i++)
  {
    GetParam(kParamOsc0Fine + i)
        ->InitDouble(std::string("Osc" + std::to_string(i + 1) + " Fine").c_str(),
                     0.0,
                     -1.0,
                     +1.0,
                     0.01,
                     "hz");
  }

  for (int i = 0; i < 4; i++)
  {
  }
  for (int i = 0; i < 4; i++)
  {
    GetParam(kParamOsc0PulseWidth + i)
        ->InitDouble(std::string("Osc" + std::to_string(i + 1) + " Pulse Width").c_str(),
                     0.5,
                     0.,
                     1.0,
                     0.01,
                     "%");
  }

  GetParam(kMainVolume)->InitDouble("Volume", 1.0, 0., 1.0, 0.1, "db");

#if IPLUG_EDITOR  // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() { return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS); };

  mLayoutFunc = [&](IGraphics *pGraphics)
  {
    const IBitmap knobLittleBitmap = pGraphics->LoadBitmap(PNGKNBPROPHETBLACK_FN, 127);
    const IBitmap knobOscBitmap = pGraphics->LoadBitmap(PNGOSC_FN, 3, false);
    const IBitmap knbProphetBlackBitmap = pGraphics->LoadBitmap(PNGKNBPROPHETBLACK_FN, 127);
    const IBitmap knbProphetSilverBitmap = pGraphics->LoadBitmap(PNGKNBPROPHETSILVER_FN, 127);
    const IBitmap btnProphetBlackBitmap = pGraphics->LoadBitmap(PNGBLACKLED_FN, 2);

    pGraphics->SetLayoutOnResize(true);
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    // Background
    pGraphics->LoadBitmap(BACKGROUND_FN, 1, true);
    pGraphics->AttachBackground(BACKGROUND_FN);

    // Oscillator buttons
    pGraphics->AttachControl(new IBSwitchControl(125, 122, btnProphetBlackBitmap, kParamOsc0));
    pGraphics->AttachControl(new IBSwitchControl(155, 122, btnProphetBlackBitmap, kParamOsc0 + 1));
    pGraphics->AttachControl(new IBSwitchControl(125, 170, btnProphetBlackBitmap, kParamOsc0 + 2));
    pGraphics->AttachControl(new IBSwitchControl(155, 170, btnProphetBlackBitmap, kParamOsc0 + 3));
    pGraphics->AttachControl(new IBSwitchControl(225, 122, btnProphetBlackBitmap, kParamOsc0 + 4));
    pGraphics->AttachControl(new IBSwitchControl(255, 122, btnProphetBlackBitmap, kParamOsc0 + 5));
    pGraphics->AttachControl(new IBSwitchControl(225, 170, btnProphetBlackBitmap, kParamOsc0 + 6));
    pGraphics->AttachControl(new IBSwitchControl(255, 170, btnProphetBlackBitmap, kParamOsc0 + 7));
    pGraphics->AttachControl(new IBSwitchControl(325, 122, btnProphetBlackBitmap, kParamOsc0 + 8));
    pGraphics->AttachControl(new IBSwitchControl(355, 122, btnProphetBlackBitmap, kParamOsc0 + 9));
    pGraphics->AttachControl(new IBSwitchControl(325, 170, btnProphetBlackBitmap, kParamOsc0 + 10));
    pGraphics->AttachControl(new IBSwitchControl(355, 170, btnProphetBlackBitmap, kParamOsc0 + 11));
    pGraphics->AttachControl(new IBSwitchControl(425, 122, btnProphetBlackBitmap, kParamOsc0 + 12));
    pGraphics->AttachControl(new IBSwitchControl(455, 122, btnProphetBlackBitmap, kParamOsc0 + 13));
    pGraphics->AttachControl(new IBSwitchControl(425, 170, btnProphetBlackBitmap, kParamOsc0 + 14));
    pGraphics->AttachControl(new IBSwitchControl(455, 170, btnProphetBlackBitmap, kParamOsc0 + 15));

    // Oscilator group volume
    pGraphics->AttachControl(new IBKnobControl(110, 270, knbProphetBlackBitmap, kParamOsc0Vol));
    pGraphics->AttachControl(new IBKnobControl(210, 270, knbProphetBlackBitmap, kParamOsc0Vol + 1));
    pGraphics->AttachControl(new IBKnobControl(310, 270, knbProphetBlackBitmap, kParamOsc0Vol + 2));
    pGraphics->AttachControl(new IBKnobControl(410, 270, knbProphetBlackBitmap, kParamOsc0Vol + 3));

    // Oscilator group frequency
    pGraphics->AttachControl(new IBKnobControl(110, 370, knbProphetBlackBitmap, kParamOsc0Freq));
    pGraphics->AttachControl(
        new IBKnobControl(210, 370, knbProphetBlackBitmap, kParamOsc0Freq + 1));
    pGraphics->AttachControl(
        new IBKnobControl(310, 370, knbProphetBlackBitmap, kParamOsc0Freq + 2));
    pGraphics->AttachControl(
        new IBKnobControl(410, 370, knbProphetBlackBitmap, kParamOsc0Freq + 3));

    // Oscilator group fine
    pGraphics->AttachControl(new IBKnobControl(110, 470, knbProphetBlackBitmap, kParamOsc0Fine));
    pGraphics->AttachControl(
        new IBKnobControl(210, 470, knbProphetBlackBitmap, kParamOsc0Fine + 1));
    pGraphics->AttachControl(
        new IBKnobControl(310, 470, knbProphetBlackBitmap, kParamOsc0Fine + 2));
    pGraphics->AttachControl(
        new IBKnobControl(410, 470, knbProphetBlackBitmap, kParamOsc0Fine + 3));

    // Oscilator group pulse width
    pGraphics->AttachControl(
        new IBKnobControl(110, 570, knbProphetBlackBitmap, kParamOsc0PulseWidth));
    pGraphics->AttachControl(
        new IBKnobControl(210, 570, knbProphetBlackBitmap, kParamOsc0PulseWidth + 1));
    pGraphics->AttachControl(
        new IBKnobControl(310, 570, knbProphetBlackBitmap, kParamOsc0PulseWidth + 2));
    pGraphics->AttachControl(
        new IBKnobControl(410, 570, knbProphetBlackBitmap, kParamOsc0PulseWidth + 3));

    // Envelope
    pGraphics->AttachControl(new IBKnobControl(710, 160, knobLittleBitmap, kParamAttack));
    pGraphics->AttachControl(new IBKnobControl(810, 160, knobLittleBitmap, kParamDecay));
    pGraphics->AttachControl(new IBKnobControl(910, 160, knobLittleBitmap, kParamSustain));
    pGraphics->AttachControl(new IBKnobControl(1010, 160, knobLittleBitmap, kParamRelease));

    // Master volume
    pGraphics->AttachControl(new IBKnobControl(1500, 160, knbProphetSilverBitmap, kMainVolume));

#if 0   // Show a keyboard or not
    //
    // Keyboard begins
    //
    const IRECT b = pGraphics->GetBounds().GetPadded(-20.f);
    const IRECT lfoPanel = b.GetFromLeft(300.f).GetFromTop(200.f);
    IRECT keyboardBounds = b.GetFromBottom(300);
    IRECT wheelsBounds = keyboardBounds.ReduceFromLeft(100.f).GetPadded(-10.f);

    pGraphics->AttachControl(new IVKeyboardControl(keyboardBounds), kCtrlTagKeyboard);
    pGraphics
        ->AttachControl(new IVButtonControl(
            keyboardBounds.GetFromTRHC(200, 30).GetTranslated(0, -30),
            SplashClickActionFunc,
            "Show/Hide Keyboard",
            DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE).WithLabelText({ 15.f, EVAlign::Middle })))
        ->SetAnimationEndActionFunction(
            [pGraphics](IControl *pCaller)
            {
              static bool hide = false;
              pGraphics->GetControlWithTag(kCtrlTagKeyboard)->Hide(hide = !hide);
              pGraphics->Resize(PLUG_WIDTH,
                                hide ? PLUG_HEIGHT / 2 : PLUG_HEIGHT,
                                pGraphics->GetDrawScale());
            });
    //
    // Keyboard ends
    //
#endif  // Show a keyboard or not
  };
#endif
}

#if IPLUG_DSP

void
ProphetW::OnReset()
{
  for (int i = 0; i < 10; ++i)
  {
    mSynth[i].setSampleRate(static_cast<long>(GetSampleRate()));
  }
}
#endif


#if IPLUG_DSP
void
ProphetW::ProcessBlock(sample **inputs, sample **outputs, int nFrames)
{
  // Channel declaration.
  PLUG_SAMPLE_DST *out01 = outputs[0];
  PLUG_SAMPLE_DST *out02 = outputs[1];

  for (int s = 0; s < nFrames; s++)
  {
    while (!mMidiQueue.Empty())
    {
      IMidiMsg msg = mMidiQueue.Peek();
      if (msg.StatusMsg() == IMidiMsg::kNoteOn)
      {
        assert(msg.NoteNumber() != 0);
        short useVoice = -1;
        // Allocate a voice for the note.
        for (int i = 0; i < 10; ++i)
        {
          if (mVoices[i] == -1)
          {
            useVoice = i;
            mVoices[i] = msg.NoteNumber();
            break;
          }
        }
        if (useVoice != -1)
        {
          mSynth[useVoice].NoteOn(msg.NoteNumber());
        }
      }
      else if (msg.StatusMsg() == IMidiMsg::kNoteOff)
      {
        short unUseVoice;
        for (int i = 0; i < 10; ++i)
        {
          if (mVoices[i] == msg.NoteNumber())
          {
            unUseVoice = i;
            mVoices[i] = -1;
            break;
          }
        }
        mSynth[unUseVoice].NoteOff(msg.NoteNumber());
      }
      mMidiQueue.Remove();
    }
    double allLeft = 0.0;
    double allRight = 0.0;
    for (int i = 0; i < 10; ++i)
    {
      allLeft += mSynth[i].getLeft();
      allRight += mSynth[i].getRight();
    }
    *out01++ = allLeft;
    *out02++ = allRight;
  }
}

void
ProphetW::ProcessMidiMsg(const IMidiMsg &msg)
{
  TRACE;
  mMidiQueue.Add(msg);  // Take care of MIDI events in ProcessBlock()
}
//void ProphetW::OnParamChange(int paramIdx)
void
ProphetW::OnParamChangeUI(int paramIdx, EParamSource source)
{
  //if (source != kUI)
  //{
  //  return;
  //}

  double value = GetParam(paramIdx)->Value();

  OutputDebugStringA(std::string("paramIdx = " + std::to_string(paramIdx) + "\n").c_str());
  OutputDebugStringA(std::string("value = " + std::to_string(value) + "\n").c_str());


  if (paramIdx >= kParamOsc0 && paramIdx <= kParamOsc0 + 15)
  {
    for (int i = 0; i < 10; ++i)
    {
      mSynth[i].m_osc[paramIdx - kParamOsc0].setIsOn(value == 1.0 ? true : false);
    }
  }

  if (paramIdx >= kParamOsc0Vol && paramIdx <= kParamOsc0Vol + 3)
  {
    for (int i = 0; i < 10; ++i)
    {
      mSynth[i].setOscVol(paramIdx - kParamOsc0Vol, value);
    }
  }

  if (paramIdx >= kParamOsc0Freq && paramIdx <= kParamOsc0Freq + 3)
  {
    for (int i = 0; i < 10; ++i)
    {
      mSynth[i].setOscFreq(paramIdx - kParamOsc0Freq, value);
    }
  }

  if (paramIdx >= kParamOsc0Fine && paramIdx <= kParamOsc0Fine + 3)
  {
    for (int i = 0; i < 10; ++i)
    {
      mSynth[i].setOscFine(paramIdx - kParamOsc0Fine, value);
    }
  }

  if (paramIdx >= kParamOsc0PulseWidth && paramIdx <= kParamOsc0PulseWidth + 3)
  {
    for (int i = 0; i < 10; ++i)
    {
      mSynth[i].setOscPulseWidth(paramIdx - kParamOsc0PulseWidth, value);
    }
  }

  switch (paramIdx)
  {
    case kParamAttack:
      for (int i = 0; i < 10; ++i)
      {
        mSynth[i].setEnvelope(Envelope::kAttack, value);
      }
      break;
    case kParamDecay:
      for (int i = 0; i < 10; ++i)
      {
        mSynth[i].setEnvelope(Envelope::kDecay, value);
      }
      break;
    case kParamSustain:
      for (int i = 0; i < 10; ++i)
      {
        mSynth[i].setEnvelope(Envelope::kSustain, value);
      }
      break;
    case kParamRelease:
      for (int i = 0; i < 10; ++i)
      {
        mSynth[i].setEnvelope(Envelope::kRelease, value);
      }
      break;
    case kMainVolume:
      for (int i = 0; i < 10; ++i)
      {
        mSynth[i].setMasterVolume(value);
      }
      break;
  }
}

#endif
