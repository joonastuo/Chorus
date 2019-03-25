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

class Chorus
{
public:
    //==============================================================================
	// Plublic methods
	Chorus(AudioProcessorValueTreeState& state);
	~Chorus();
	void prepare(const float& sampleRate, const int& samplesPerBlock, const int& numChannels);
	void process(AudioBuffer<float>& buffer);

private:
    //==============================================================================
	// Private methods
	float linearInterp(const float& y0, const float& yp1, const float& frac);

    //==============================================================================
	// Private member variables
	AudioProcessorValueTreeState& mState;
	// Two delay lines, LFOs and smoothed gain valus.
	// One for each delay line (2 delay lines)
	const int mNumDelayLines = 2;
	std::array<DelayLine<float>,	 2> delayLines;
	std::array<LFO,					 2> mLFOs;
	std::array<SmoothedValue<float>, 2> mSmoothG;
	std::array<SmoothedValue<float>, 2> mSmoothW;
	std::array<SmoothedValue<float>, 2> mSmoothFB;
	// Audio buffer info
	float mSampleRate = 44100.f;
	int mSamplesPerBlock = 512;
	int mNumChannels = 2;
	int mDelayBufferLen = 0;
};