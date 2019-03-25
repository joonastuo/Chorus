/*
  ==============================================================================

    Chorus.cpp
    Created: 25 Mar 2019 12:02:57pm
    Author:  Joonas

  ==============================================================================
*/

#include "Chorus.h"

//==============================================================================
Chorus::Chorus(AudioProcessorValueTreeState& state)
	: mState(state)
{
	// Empty constructor
}

//==============================================================================
Chorus::~Chorus()
{
	// Empty destructor
}

//==============================================================================
void Chorus::prepare(const float & sampleRate, const int & samplesPerBlock, const int & numChannels)
{
	// Set values to member variables ===============
	mSampleRate = sampleRate;
	mSamplesPerBlock = samplesPerBlock;
	mNumChannels = numChannels;
	// Set to be maximum delay value + safety!!!
	mDelayBufferLen = 2 * (mSampleRate + mSamplesPerBlock);

	// Prepare delay lines ==========================
	for (auto& dline : delayLines)
	{
		dline.resize(mDelayBufferLen);
		dline.clear();
	}

	// Prepare LFOs for delay lines =================
	for (auto& lfo : mLFOs)
	{
		lfo.prepare(sampleRate, samplesPerBlock);
		lfo.setWaveform(0);
		lfo.setUnipolar(true);
		lfo.setFreq(5);
	}

	// Set phase shift to first delay line ==========
	float phase = *mState.getRawParameterValue(IDs::lfoPhase);
	mLFOs[0].setPhase(phase);

	// Prepare smoothed values ======================
	float feedback = *mState.getRawParameterValue(IDs::feedback);
	float wet	   = *mState.getRawParameterValue(IDs::wetness);

	float FB = feedback / 100.f;
	float W  = (wet / 100.f) * 0.5f;
	float G  = 1 - W;

	for (int delayLine = 0; delayLine < mNumDelayLines; ++delayLine)
	{
		mSmoothFB[delayLine].reset(mSamplesPerBlock);
		mSmoothW[delayLine] .reset(mSamplesPerBlock);
		mSmoothG[delayLine] .reset(mSamplesPerBlock);

		mSmoothFB[delayLine].setCurrentAndTargetValue(FB);
		mSmoothW[delayLine] .setCurrentAndTargetValue(W);
		mSmoothG[delayLine] .setCurrentAndTargetValue(G);
	}
}

//==============================================================================
void Chorus::process(AudioBuffer<float>& buffer)
{
	// Get user parameters ==========================
	float feedback = *mState.getRawParameterValue(IDs::feedback);
	float wet = *mState.getRawParameterValue(IDs::wetness);
	float time = *mState.getRawParameterValue(IDs::time);
	float lfoFreq = *mState.getRawParameterValue(IDs::lfoFreq);
	float phase = *mState.getRawParameterValue(IDs::lfoPhase);
	int waveform = *mState.getRawParameterValue(IDs::lfoWaveform);
	bool effectOn = *mState.getRawParameterValue(IDs::onOff);

	// Set phase shift to first channel =============
	mLFOs[0].setPhase(phase);

	// Transform values form % to values from 0 to 1
	float FB = feedback / 100.f;
	// Wet/dry from 0/100 to 50/50
	float W = (wet / 100.f) * 0.5f;
	float G = 1 - W;

	// Set gain values ==============================
	for (int channel = 0; channel < 2; ++channel)
	{
		mSmoothFB[channel].setValue(FB);
		mSmoothW[channel].setValue(W);
		mSmoothG[channel].setValue(G);
	}

	// Effect processing loop =======================
	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		// Get channel LFO
		auto& lfo = mLFOs[channel];
		lfo.setFreq(lfoFreq);
		lfo.setWaveform(waveform);

		// Get channel input and output buffer
		const float* input = buffer.getReadPointer(channel);
		float* output = buffer.getWritePointer(channel);

		// Iterate trough samples in a channel
		for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			// Get smoothed gain values
			FB = mSmoothFB[channel].getNextValue();
			W = mSmoothW[channel].getNextValue();
			G = mSmoothG[channel].getNextValue();

			// Get delay amount
			float delayTime = time * lfo.getValue();
			lfo.advanceSamples(1);
			float delayInSamplesFrac = mSampleRate * (delayTime / 1000.f);
			int delayInSamplesInt = static_cast<int> (delayInSamplesFrac);
			float delayFrac = delayInSamplesFrac - delayInSamplesInt;

			// Get delayed sample
			float delayedSample = 0.f;
			// Fractal delay with liner interpolation if neede
			if (delayFrac != 0)
			{
				float y0 = delayLines[channel].get(delayInSamplesInt);
				float ym1 = delayLines[channel].get(delayInSamplesInt + 1);
				delayedSample = linearInterp(y0, ym1, delayFrac);
			}
			else
			{
				delayedSample = delayLines[channel].get(delayInSamplesInt);
			}

			auto inputSample = input[sample];

			// Add input and feedback to delayLine
			delayLines[channel].push(inputSample + delayedSample * FB);

			// Mix dry and wet input
			output[sample] = inputSample * G + delayedSample * W;
		}
	}

}

//==============================================================================
float Chorus::linearInterp(const float & y0, const float & yp1, const float & frac)
{
	return yp1 * frac + y0 * (1 - frac);
}
