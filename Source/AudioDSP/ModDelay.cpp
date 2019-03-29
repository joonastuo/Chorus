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

void ModDelay::prepareDelay(const int& delayLineSize)
{
	mDelayLine.resize(delayLineSize);
	mDelayLine.clear();
}

void ModDelay::prepareLFO(const float& sampleRate, const float& samplesPerBlock, const float& phase, const float& freq, const float& depth)
{
	mSampleRate = sampleRate;
	mLFO.prepare(sampleRate, samplesPerBlock);
	mLFO.setWaveform(Waveforms::tri);
	mLFO.setUnipolar(true);
	mLFO.setPhase(phase);
	mLFO.setFreq(freq);
	mLFO.setDepth(depth);
}

void ModDelay::prepareGain(const int& samplesPerBlock, const float & W, const float & FB)
{
	mSamplesPerBlock = samplesPerBlock;

	mSmoothW.reset(samplesPerBlock);
	mSmoothG.reset(samplesPerBlock);
	mSmoothFB.reset(samplesPerBlock);
	float G = 1 - W;
	mSmoothW.setCurrentAndTargetValue(W);
	mSmoothG.setCurrentAndTargetValue(G);
	mSmoothFB.setCurrentAndTargetValue(FB);
}

void ModDelay::prepareFilters(const int & sampleRate, const float & lpFreq, const float & hpFreq)
{
	IIRCoefficients hpCoeff = IIRCoefficients::makeHighPass(sampleRate, hpFreq);
	mHighPass.setCoefficients(hpCoeff);
	IIRCoefficients lpCoeff = IIRCoefficients::makeLowPass(sampleRate, lpFreq);
	mLowPass.setCoefficients(lpCoeff);
	mHighPass.reset();
	mLowPass.reset();
}

void ModDelay::update(const float & LFOFreq, const float& LFODepth, const float & W, const float & FB, const float& lpFreq, const float& hpFreq)
{
	mLFO.setFreq(LFOFreq);
	mLFO.setDepth(LFODepth);
	float G = W - 1;
	mSmoothW.setTargetValue(W);
	mSmoothG.setCurrentAndTargetValue(G);
	mSmoothFB.setTargetValue(FB);

	IIRCoefficients hpCoeff = IIRCoefficients::makeHighPass(mSampleRate, hpFreq);
	mHighPass.setCoefficients(hpCoeff);
	IIRCoefficients lpCoeff = IIRCoefficients::makeLowPass(mSampleRate, lpFreq);
	mLowPass.setCoefficients(lpCoeff);
}

void ModDelay::process(const float * input, float * output)
{
	float G = 0.f;
	float W = 0.f;
	float FB = 1.f;

	// Iterate trough samples in a channel
	for (auto sample = 0; sample < mSamplesPerBlock; ++sample)
	{
		// Get smoothed gain values
		FB = mSmoothFB.getNextValue();
		W = mSmoothW.getNextValue();
		G = mSmoothG.getNextValue();

		// Get delay amount
		float delayTime = mDelayTime * mLFO.getValue();
		mLFO.advanceSamples(1);
		float delayInSamplesFrac = mSampleRate * (delayTime / 1000.f);
		int delayInSamplesInt = static_cast<int> (delayInSamplesFrac);
		float delayFrac = delayInSamplesFrac - delayInSamplesInt;

		// Get delayed sample
		float delayedSample = 0.f;
		// Fractal delay with liner interpolation if neede
		if (delayFrac != 0)
		{
			float y0 = mDelayLine.get(delayInSamplesInt);
			float ym1 = mDelayLine.get(delayInSamplesInt + 1);
			delayedSample = linearInterp(y0, ym1, delayFrac);
		}
		else
		{
			delayedSample = mDelayLine.get(delayInSamplesInt);
		}

		auto inputSampleW = input[sample];
		auto inputSampleG = input[sample];
		inputSampleG = mLowPass.processSingleSampleRaw(inputSampleG);
		inputSampleW = mHighPass.processSingleSampleRaw(inputSampleW);

		// Add input and feedback to delayLine
		mDelayLine.push(inputSampleW + delayedSample * FB);

		// Mix dry and wet input
		output[sample] = inputSampleG * G + delayedSample * W;
	}
}

//==============================================================================
float ModDelay::linearInterp(const float & y0, const float & yp1, const float & frac)
{
	return yp1 * frac + y0 * (1 - frac);
}
