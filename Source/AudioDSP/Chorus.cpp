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
	// Prepare delay lines
	float minDelay = 7.f;
	float maxDelayDepth = 20.f;
	int delayBufferLen = static_cast<int>(((minDelay + maxDelayDepth) / 1000.f) * sampleRate + mSamplesPerBlock);

	DelayData delayDataL;
	DelayData delayDataC;
	DelayData delayDataR;
	std::array<DelayData, 3> delayDataArray = { delayDataL, delayDataC, delayDataR };

	float lfoFreqs [3] = { 0.f, 0.f, 0.f };
	getLfoFreq(lfoFreqs);
	float lfoPhases[3] = { 0.f, 0.f, 0.f };
	getLfoPhase(lfoPhases);
	float lfoDepths[3] = { 0.f, 0.f, 0.f };
	getLfoDepth(lfoDepths);
	float feedbacks[3] = { 0.f, 0.f, 0.f };
	getFeedback(feedbacks);

	for (auto i = 0; i < 3; ++i)
	{
		delayDataArray[i].sampleRate	  = sampleRate;
		delayDataArray[i].samplesPerBlock = samplesPerBlock;
		delayDataArray[i].minDelayTime	  = minDelay;
		delayDataArray[i].maxDelayDepth   = maxDelayDepth;
		delayDataArray[i].delayLineLen	  = delayBufferLen;
		delayDataArray[i].wetness		  = getWetness();
		delayDataArray[i].feedback		  = feedbacks[i];
		delayDataArray[i].lfoFreq		  = lfoFreqs[i];
		delayDataArray[i].lfoPhase		  = lfoPhases[i];
		delayDataArray[i].lfoDepth		  = lfoDepths[i];
	}
	
	mLeftDelay  .prepare(delayDataArray[0]);
	mCenterDelay.prepare(delayDataArray[1]);
	mRightDelay .prepare(delayDataArray[2]);

	// Set values to member variables ===============
	mSampleRate = sampleRate;
	mSamplesPerBlock = samplesPerBlock;
	mNumChannels = numChannels;
	mCenterBuffer.setSize(1, samplesPerBlock);
}

//==============================================================================
void Chorus::process(AudioBuffer<float>& buffer)
{
	updateParameters();

	// Left and right input
	const float* leftInput  = buffer.getReadPointer(0);
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

//==============================================================================
void Chorus::updateParameters()
{
	float W = getWetness();

	float feedback[3] = { 0.f };
	getFeedback(feedback);

	float lfoFreq[3] = { 0.f };
	getLfoFreq(lfoFreq);

	float lfoDepth[3] = { 0.f };
	getLfoDepth(lfoDepth);

	mLeftDelay  .update(lfoFreq[0], lfoDepth[0], W, feedback[0]);
	mCenterDelay.update(lfoFreq[1], lfoDepth[1], W, feedback[1]);
	mRightDelay .update(lfoFreq[2], lfoDepth[2], W, feedback[2]);
}

//==============================================================================
float Chorus::getWetness()
{
	float W   = *mState.getRawParameterValue(IDs::wetness);
	return W / 100.f;
}

//==============================================================================
void Chorus::getFeedback(float * feedback)
{
	float FBL = *mState.getRawParameterValue(IDs::feedbackL);
	float FBC = *mState.getRawParameterValue(IDs::feedbackC);
	float FBR = *mState.getRawParameterValue(IDs::feedbackR);

	feedback[0] = FBL / 100.f;
	feedback[1] = FBC / 100.f;
	feedback[2] = FBR / 100.f;
}

//==============================================================================
void Chorus::getLfoFreq(float * lfoFreq)
{
	float lfoPercentageL = *mState.getRawParameterValue(IDs::lfoFreqL);
	float lfoPercentageC = *mState.getRawParameterValue(IDs::lfoFreqC);
	float lfoPercentageR = *mState.getRawParameterValue(IDs::lfoFreqR);

	lfoFreq[0] = 0.02 * exp(lfoPercentageL * 0.05521f);
	lfoFreq[1] = 0.02 * exp(lfoPercentageC * 0.05521f);
	lfoFreq[2] = 0.02 * exp(lfoPercentageR * 0.05521f);
}

//==============================================================================
void Chorus::getLfoPhase(float * lfoPhase)
{
	lfoPhase[0] = 90.f;
	lfoPhase[1] = 0.f;
	lfoPhase[2] = 270.f;
}

//==============================================================================
void Chorus::getLfoDepth(float * lfoDepth)
{
	lfoDepth[0] = *mState.getRawParameterValue(IDs::lfoDepthL);
	lfoDepth[1] = *mState.getRawParameterValue(IDs::lfoDepthC);
	lfoDepth[2] = *mState.getRawParameterValue(IDs::lfoDepthR);
}

