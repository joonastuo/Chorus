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

class ModDelay
{
public:
	ModDelay();
	~ModDelay();
	void prepareDelay(const int& delayLineSize);
	void prepareLFO(const float& sampleRate, const float& samplesPerBlock, const float& phase, const float& freq, const float& depth);
	void prepareGain(const int& samplesPerBlock, const float& W, const float& FB);
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
	float mDelayTime = 7.f;
};