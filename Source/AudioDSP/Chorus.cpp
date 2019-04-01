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
	float W = *mState.getRawParameterValue(IDs::wetness);
	float FBL = *mState.getRawParameterValue(IDs::feedbackL);
	float FBC = *mState.getRawParameterValue(IDs::feedbackC);
	float FBR = *mState.getRawParameterValue(IDs::feedbackR);

	// Set values to member variables ===============
	mSampleRate = sampleRate;
	mSamplesPerBlock = samplesPerBlock;
	mNumChannels = numChannels;
	// Set to be maximum delay value + safety!!!
	mDelayBufferLen = 2 * (mSampleRate + mSamplesPerBlock);
	mCenterBuffer.setSize(1, samplesPerBlock);

	mLeftDelay.prepareDelay(mDelayBufferLen);
	mCenterDelay.prepareDelay(mDelayBufferLen);
	mRightDelay.prepareDelay(mDelayBufferLen);

	mLeftDelay.prepareGain(samplesPerBlock, W, FBL);
	mCenterDelay.prepareGain(samplesPerBlock, W, FBC);
	mRightDelay.prepareGain(samplesPerBlock, W, FBR);

	float lfoFreqL = *mState.getRawParameterValue(IDs::lfoFreqL);
	float lfoFreqC = *mState.getRawParameterValue(IDs::lfoFreqC);
	float lfoFreqR = *mState.getRawParameterValue(IDs::lfoFreqR);

	float lfoDepthL = *mState.getRawParameterValue(IDs::lfoDepthL);
	float lfoDepthC = *mState.getRawParameterValue(IDs::lfoDepthC);
	float lfoDepthR = *mState.getRawParameterValue(IDs::lfoDepthR);

	float lfoPhaseL = 90.f;
	float lfoPhaseC = 0.f;
	float lfoPhaseR = 270.f;

	mLeftDelay.prepareLFO(mSampleRate, samplesPerBlock, lfoPhaseL, lfoFreqL, lfoDepthL);
	mCenterDelay.prepareLFO(mSampleRate, samplesPerBlock, lfoPhaseC, lfoFreqC, lfoDepthC);
	mRightDelay.prepareLFO(mSampleRate, samplesPerBlock, lfoPhaseR, lfoFreqR, lfoDepthR);
}

//==============================================================================
void Chorus::process(AudioBuffer<float>& buffer)
{
	updateParameters();

	// Left and right input
	const float* leftInput = buffer.getReadPointer(0);
	const float* rightInput = buffer.getReadPointer(0);
	if (buffer.getNumChannels() == 2)
		rightInput = buffer.getReadPointer(1);
	else if (buffer.getNumChannels() > 2)
		return;

	// Middle
	mCenterBuffer.copyFromWithRamp(0, 0, leftInput, buffer.getNumSamples(), 0.5f, 0.5f);
	mCenterBuffer.copyFromWithRamp(0, 0, rightInput, buffer.getNumSamples(), 0.5f, 0.5f);
	
	const float* centerInput = mCenterBuffer.getWritePointer(0);

	float* leftOutput = buffer.getWritePointer(0);
	float* rightOutput = buffer.getWritePointer(1);
	float* centerOutput = mCenterBuffer.getWritePointer(0);

	mLeftDelay  .process(leftInput  , leftOutput);
	mCenterDelay.process(centerInput, centerOutput);
	mRightDelay .process(rightInput , rightOutput);

	buffer.addFrom(0, 0, centerOutput, mCenterBuffer.getNumSamples());
	buffer.addFrom(1, 0, centerOutput, mCenterBuffer.getNumSamples());
}

void Chorus::updateParameters()
{
	float wetness = *mState.getRawParameterValue(IDs::wetness);

	float W = wetness / 100.f;

	float FeedbackL = *mState.getRawParameterValue(IDs::feedbackL);
	float FeedbackC = *mState.getRawParameterValue(IDs::feedbackC);
	float FeedbackR = *mState.getRawParameterValue(IDs::feedbackR);

	float FBL = FeedbackL / 100.f;
	float FBC = FeedbackC / 100.f;
	float FBR = FeedbackR / 100.f;

	float lfoFreqL = *mState.getRawParameterValue(IDs::lfoFreqL);
	float lfoFreqC = *mState.getRawParameterValue(IDs::lfoFreqC);
	float lfoFreqR = *mState.getRawParameterValue(IDs::lfoFreqR);

	float lfoDepthL = *mState.getRawParameterValue(IDs::lfoDepthL);
	float lfoDepthC = *mState.getRawParameterValue(IDs::lfoDepthC);
	float lfoDepthR = *mState.getRawParameterValue(IDs::lfoDepthR);

	mLeftDelay.update(lfoFreqL, lfoDepthL, W, FBL);
	mCenterDelay.update(lfoFreqC, lfoDepthC, W, FBC);
	mRightDelay.update(lfoFreqR, lfoDepthR, W, FBR);
}
