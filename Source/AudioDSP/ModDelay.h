/*
  ==============================================================================

    ModDelay.h
    Created: 25 Mar 2019 7:57:26pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayLine.h"
#include "LFO.h"
#include "../Helper/Identifiers.h"

//==============================================================================
struct DelayData
{
	float sampleRate = 44100.f;
	int samplesPerBlock = 512;
	float lfoFreq = 0.f;
	float lfoPhase = 0.f;
	float lfoDepth = 0.f;
	float wetness = 0.f;
	float feedback = 0.f;
	float minDelayTime = 0.f;
	float maxDelayDepth = 0.f;
	int   delayLineLen = 0;
};

//==============================================================================
class ModDelay
{
public:
	ModDelay();
	~ModDelay();
	void prepare(DelayData data);
	void update(const float& LFOFreq, const float& LFODepth, const float& W, const float& FB);
	void process(const float* input, float* output);

private:
	float linearInterp(const float& y0, const float& yp1, const float& frac);

	DelayLine<float> mDelayLine;
	LFO mLFO;
	SmoothedValue<float> mSmoothG;
	SmoothedValue<float> mSmoothW;
	SmoothedValue<float> mSmoothFB;

	float mSampleRate = 44100.f;
	int mSamplesPerBlock = 512;

	float mMinDelayTime = 7.f;
	float mMaxDelayDepth = 20.f;
};