/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LookAndFeel/KnobLookAndFeel.h"

//==============================================================================
/**
*/
class ChorusAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ChorusAudioProcessorEditor (ChorusAudioProcessor&);
    ~ChorusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
	void drawTitleLines(Graphics& g, Rectangle<float> area);
	void drawLegs(Graphics& g, Rectangle<float> area);
    void resized() override;

private:
    //==============================================================================
	void initialiseGUI();

    //==============================================================================
    ChorusAudioProcessor& processor;
	AudioProcessorValueTreeState& mState;

	// Look and feel =====================
	// Component sizes
	const int mSliderWidth = 80;
	const int mSliderHeight = 80;
	const int mTextBoxWidth = mSliderWidth;
	const int mTextBoxHeight = 22;
	const int mLabelHeight = 26;
	const int mLabelWidht = mSliderWidth;
	const int mTitleHeight = 40;
	const int mTitleWidth = mSliderWidth;
	const int mWindowMarginWidth = 10;
	const int mWindowMarginHeight = 10;
	const int mSpaceBetweenW = 100;
	const int mSpaceBetweenH = 100;
	// Label font
	Font mLabelFont = Font("Pacifico", mLabelHeight, Font::plain);
	Font mTitleFont = Font("Pacifico", mTitleHeight, Font::bold);
	Slider::SliderStyle mSliderStyle = Slider::SliderStyle::Rotary;
	Rectangle<int> mFlexArea;
	KnobLookAndFeel mKnobLookAndFeel;

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	// Mix ===============================
	Label mMixLabel;
	Slider mMixSlider;
	std::unique_ptr<SliderAttachment> mMixSliderAttachment;

	// Low pass iflter ===================
	Label mHighPassLabel;
	Slider mHighPassSlider;
	std::unique_ptr<SliderAttachment> mHighPassSliderAttachment;
	
	// High pass filter ==================
	Label mLowPassLabel;
	Slider mLowPassSlider;
	std::unique_ptr<SliderAttachment> mLowPassSliderAttachment;

	// LFO frequency ======================
	// Left
	Slider mLfoFreqLSlider;
	std::unique_ptr<SliderAttachment> mLfoFreqLSliderAttachment;
	// Center
	Slider mLfoFreqCSlider;
	std::unique_ptr<SliderAttachment> mLfoFreqCSliderAttachment;
	// Right
	Slider mLfoFreqRSlider;
	std::unique_ptr<SliderAttachment> mLfoFreqRSliderAttachment;
	// LFO depth =========================
	// Left
	Slider mLfoDepthLSlider;
	std::unique_ptr<SliderAttachment> mLfoDepthLSliderAttachment;
	// Center
	Slider mLfoDepthCSlider;
	std::unique_ptr<SliderAttachment> mLfoDepthCSliderAttachment;
	// Right
	Slider mLfoDepthRSlider;
	std::unique_ptr<SliderAttachment> mLfoDepthRSliderAttachment;

	// Feedback ==========================
	// Left
	Slider mFeedbackLSlider;
	std::unique_ptr<SliderAttachment> mFeedbackLSliderAttachment;
	// Center
	Slider mFeedbackCSlider;
	std::unique_ptr<SliderAttachment> mFeedbackCSliderAttachment;
	// Right
	Slider mFeedbackRSlider;
	std::unique_ptr<SliderAttachment> mFeedbackRSliderAttachment;

	Label mLeftTitleLabel;
	Label mCenterTitleLabel;
	Label mRightTitleLabel;
	Label mFeedbackTitleLabel;
	Label mDepthTitleLabel;
	Label mFreqTitleLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusAudioProcessorEditor)
};
