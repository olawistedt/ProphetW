

//
// Examples  of filters is LP12, LP24, HP12, BP12, LPDL (examples from the Synth1 synth)
//

#include "Filter.h"

#define _USE_MATH_DEFINES  // To get M_PI
#include <math.h>

// cutoff = cutoff freq in Hz
// fs = sampling frequency //(e.g. 44100Hz)
// res = resonance [0 - 1] //(minimum - maximum)
#if 0
Moog(double cutoff, double fs, double res)
{
  //Init
  f = 2 * cutoff / fs; //[0 - 1]
  k = 3.6*f - 1.6*f*f -1; //(Empirical tunning)
  p = (k+1)*0.5;
//  scale = e^((1-p)*1.386249;
  r = res*scale;
  y4 = output;
  
  double y1=y2=y3=y4=oldx=oldy1=oldy2=oldy3=0.0;
  
  //Loop
  //--Inverted feed back for corner peaking
  x = input - r*y4;
  
  //Four cascaded onepole filters (bilinear transform)
  y1=x*p + oldx*p - k*y1;
  y2=y1*p+oldy1*p - k*y2;
  y3=y2*p+oldy2*p - k*y3;
  y4=y3*p+oldy3*p - k*y4;
  
  //Clipper band limited sigmoid
  y4 = y4 - (y4^3)/6;
  
  oldx = x;
  oldy1 = y1;
  oldy2 = y2;
  oldy3 = y3;
}


double lowPassFilter(double x, double sampleRate, double cutoffFrequency) {
    static double y = 0.0;
    const double dt = 1.0 / sampleRate;
    const double RC = 1.0 / (2.0 * M_PI * cutoffFrequency);
    const double alpha = dt / (RC + dt);
    y += alpha * (x - y);
    return y;
}

double lowPassFilter24db(double input, double cutoffFrequency, double sampleRate) {
    static double prevOutput[3] = {0};
    double deltaTime = 1.0 / sampleRate;
    double RC = 1.0 / (cutoffFrequency * 2 * M_PI);
    double alpha = deltaTime / (RC + deltaTime);
    double output = alpha * input + (1 - alpha) * (prevOutput[0] + 2 * prevOutput[1] + prevOutput[2]) / 4;
    prevOutput[2] = prevOutput[1];
    prevOutput[1] = prevOutput[0];
    prevOutput[0] = output;
    return output;
}

double highPassFilter(double input, double cutoffFrequency, double sampleRate) {
    static double prevOutput = 0;
    double deltaTime = 1.0 / sampleRate;
    double RC = 1.0 / (cutoffFrequency * 2 * M_PI);
    double alpha = RC / (RC + deltaTime);
    double output = alpha * (prevOutput + input - prevInput);
    prevInput = input;
    prevOutput = output;
    return output;
}
#endif

Filter::Filter() : mPrevOutput(0), mPrevPrevOutput(0) {}

double
Filter::lowPassFilter12db(double input, double cutoffFrequency, double sampleRate)
{
  static double prevOutput = 0;
  static double prevPrevOutput = 0;
  double deltaTime = 1.0 / sampleRate;
  double RC = 1.0 / (cutoffFrequency * 2 * M_PI);
  double alpha = deltaTime / (RC + deltaTime);
  double output = alpha * input + (1 - alpha) * (prevOutput + prevPrevOutput) / 2;
  prevPrevOutput = prevOutput;
  prevOutput = output;
  return output;
}


MoogFilter::MoogFilter()
{
  fs = 44100.0;

  init();
}

MoogFilter::~MoogFilter() {}

void
MoogFilter::init()
{
  // initialize values
  y1 = y2 = y3 = y4 = oldx = oldy1 = oldy2 = oldy3 = 0;
  calc();
};

void
MoogFilter::calc()
{
  double f = (cutoff + cutoff) / fs;  //[0 - 1]
  p = f * (1.8f - 0.8f * f);
  k = p + p - 1.f;

  double t = (1.f - p) * 1.386249f;
  double t2 = 12.f + t * t;
  r = res * (t2 + 6.f * t) / (t2 - 6.f * t);
};

double
MoogFilter::process(double input)
{
  // process input
  x = input - r * y4;

  //Four cascaded onepole filters (bilinear transform)
  y1 = x * p + oldx * p - k * y1;
  y2 = y1 * p + oldy1 * p - k * y2;
  y3 = y2 * p + oldy2 * p - k * y3;
  y4 = y3 * p + oldy3 * p - k * y4;

  //Clipper band limited sigmoid
  y4 -= (y4 * y4 * y4) / 6.f;

  oldx = x;
  oldy1 = y1;
  oldy2 = y2;
  oldy3 = y3;
  return y4;
}

double
MoogFilter::getCutoff()
{
  return cutoff;
}

void
MoogFilter::setCutoff(double c)
{
  cutoff = c;
  calc();
}

double
MoogFilter::getRes()
{
  return res;
}

void
MoogFilter::setRes(double r)
{
  res = r;
  calc();
}

void
MoogFilter::setSampleRate(double sr)
{
  fs = sr;
  calc();
}
