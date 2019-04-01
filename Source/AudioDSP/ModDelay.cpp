/*
  ==============================================================================

    ModDelay.cpp
    Created: 25 Mar 2019 7:57:26pm
    Author:  Joonas

  ==============================================================================
*/

#include "ModDelay.h"

ModDelay::ModDelay()
{
	// Empty constructor
}

ModDelay::~ModDelay()
{
	// Empty destructor
}

//==============================================================================
void ModDelay::prepare(DelayData data)
{
	mSampleRate = data.sampleRate;
	mSamplesPerBlock = data.samplesPerBlock;
	// Delay line
	mDelayLine.resize(data.delayLineLen);
	mDelayLine.clear();
	// LFO
	mLFO.prepare(mSampleRate, mSamplesPerBlock);
	mLFO.setWaveform(Waveforms::tri);
	mLFO.setUnipolar(true);
	mLFO.setPhase(data.lfoPhase);
	mLFO.setFreq(data.lfoFreq);
	mLFO.setDepth(data.lfoDepth);
	// Gains
	mSmoothW.reset(mSamplesPerBlock);
	mSmoothG.reset(mSamplesPerBlock);
	mSmoothFB.reset(mSamplesPerBlock);
	float G = 1 - data.wetness;
	jassert(data.wetness > 0 || data.wetness < 1 || data.feedback > -1 || data.feedback < 1);
	mSmoothW .setCurrentAndTargetValue(data.wetness);
	mSmoothG .setCurrentAndTargetValue(G);
	mSmoothFB.setCurrentAndTargetValue(data.feedback);
}

//==============================================================================
void ModDelay::update(const float & LFOFreq, const float& LFODepth, const float& W, const float& FB)
{
	// update LFOs
	mLFO.setFreq(LFOFreq);
	mLFO.setDepth(LFODepth);
	// Update gains
	float G = W - 1;
	mSmoothW .setTargetValue(W);
	mSmoothG .setCurrentAndTargetValue(G);
	mSmoothFB.setTargetValue(FB);
}

//==============================================================================
void ModDelay::process(const float * input, float * output)
{
	float G  = 0.f;
	float W  = 0.f;
	float FB = 1.f;

	// Iterate trough samples in a channel
	for (auto sample = 0; sample < mSamplesPerBlock; ++sample)
	{
		// Get smoothed gain values
		FB = mSmoothFB.getNextValue();
		W  = mSmoothW .getNextValue();
		G  = mSmoothG .getNextValue();

		// Get delay amount
		float delayTime = mMinDelayTime + mMaxDelayDepth * mLFO.getValueAndAdvance();

		// Separate delay to int + frac
		float delayInSamplesFrac = mSampleRate * (delayTime / 1000.f);
		int   delayInSamplesInt  = static_cast<int> (delayInSamplesFrac);
		float frac = delayInSamplesFrac - delayInSamplesInt;

		// Get delayed value
		float delayedSample = 0.f;
		// Fractal delay with liner interpolation if neede
		if (frac != 0)
		{
			float y0 = mDelayLine.get(delayInSamplesInt);
			float ym1 = mDelayLine.get(delayInSamplesInt + 1);
			delayedSample = linearInterp(y0, ym1, frac);
		}
		// No fractal part
		else
		{
			delayedSample = mDelayLine.get(delayInSamplesInt);
		}

		auto inputSample = input[sample];

		// Add input and feedback to delayLine
		mDelayLine.push(inputSample + delayedSample * FB);

		// Mix dry and wet input
		output[sample] = inputSample * G + delayedSample * W;
	}
}

//==============================================================================
float ModDelay::linearInterp(const float & y0, const float & yp1, const float & frac)
{
	return yp1 * frac + y0 * (1 - frac);
}
