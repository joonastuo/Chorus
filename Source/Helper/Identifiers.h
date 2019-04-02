/*
  ==============================================================================

    Identifiers.h
    Created: 25 Mar 2019 12:02:18pm
    Author:  Joonas

  ==============================================================================
*/

#pragma once

namespace IDs {
	// JUCE Strings
	#define DECLARE_ID(name) const juce::String name (#name);

	DECLARE_ID(wetness)		// Dry / wet -ratio (0% - 100%)
	DECLARE_ID(feedbackL)	// Feedback gain (-100% - 100%)
	DECLARE_ID(feedbackC)	// Feedback gain (-100% - 100%)
	DECLARE_ID(feedbackR)	// Feedback gain (-100% - 100%)
	DECLARE_ID(lfoFreqL)		// LFO frequency (0.01 - 10 Hz)
	DECLARE_ID(lfoFreqC)		// LFO frequency (0.01 - 10 Hz)
	DECLARE_ID(lfoFreqR)		// LFO frequency (0.01 - 10 Hz)
	DECLARE_ID(lfoDepthL)
	DECLARE_ID(lfoDepthC)
	DECLARE_ID(lfoDepthR)
	DECLARE_ID(onOff)		// effect on / off
	DECLARE_ID(volume)

	#undef DECLARE_ID
}
