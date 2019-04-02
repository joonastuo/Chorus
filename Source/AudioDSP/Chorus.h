/*
  ==============================================================================

    Chorus.h
    Created: 25 Mar 2019 12:02:57pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Helper/Identifiers.h"
#include "LFO.h"
#include "DelayLine.h"
#include <array>
#include "ModDelay.h"

class Chorus
{
public:
    //==============================================================================
	// Plublic methods
	Chorus(AudioProcessorValueTreeState& state);
	~Chorus();
	void prepare(const float& sampleRate, const int& samplesPerBlock, const int& numChannels);
	void process(AudioBuffer<float>& buffer);
	void updateParameters();

private:
	float getWetness();
	void getFeedback(float* feedback);
	void getLfoFreq(float* lfoFreq);
	void getLfoPhase(float* lfoPhase);
	void getLfoDepth(float* lfoDepth);
	float getVolume();

    //==============================================================================
	// Private member variables
	AudioProcessorValueTreeState& mState;

	AudioBuffer<float> mCenterBuffer;
	// Two delay lines, LFOs and smoothed gain valus.
	// One for each delay line (2 delay lines)
	ModDelay mLeftDelay;
	ModDelay mCenterDelay;
	ModDelay mRightDelay;

	// Audio buffer info
	float mSampleRate = 44100.f;
	int mSamplesPerBlock = 512;
	int mNumChannels = 2;
	int mDelayBufferLen = 0;
};