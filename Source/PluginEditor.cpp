/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusAudioProcessorEditor::ChorusAudioProcessorEditor (ChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), mState(p.getState())
{
	int windowWidth  = 2 * mWindowMarginWidth  + 4 * jmax(mSliderWidth, mLabelWidht);
	int windowHeight = 2 * mWindowMarginHeight + 3 * mSliderHeight + mTitleHeight + mTitleHeight + mSliderHeight;
    setSize (windowWidth, windowHeight);
	initialiseGUI();
	mFlexArea = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
}

ChorusAudioProcessorEditor::~ChorusAudioProcessorEditor()
{
}

//==============================================================================
void ChorusAudioProcessorEditor::paint (Graphics& g)
{
	// Fill background
	Colour bgColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    g.fillAll (bgColour.brighter(.1f));
	
	// Plugin area
	auto area = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
	// Area for title ("LCR Chorus")
	auto titleArea = area.removeFromTop(mLabelHeight + mSliderHeight + 10.f);
	titleArea.removeFromBottom(10.f);

	// Area for the mix control
	auto mixArea = titleArea.removeFromRight(mSliderWidth);

	// Fill mix area
	Colour boxColour = Colours::aqua.darker(.5f).contrasting(.3f);
	g.setColour(boxColour);
	g.fillRoundedRectangle(mixArea.toFloat(), 10.f);
	// Draw lines around mix area
	g.setColour(bgColour.darker(0.8f));
	g.drawRoundedRectangle(mixArea.toFloat(), 10.f, 3.f);
	// Draw legs on mix area
	drawLegs(g, mixArea.toFloat());

	// Separate title area from mix
	titleArea.removeFromRight(10.f);
	titleArea.removeFromTop(45.f);
	// Draw legs on title area
	drawLegs(g, titleArea.toFloat());
	// Fill title area and draw border
	g.setColour(boxColour);
	g.fillRoundedRectangle(titleArea.toFloat(), 10.f);
	g.setColour(bgColour.darker(0.8f));
	g.drawRoundedRectangle(titleArea.toFloat(), 10.f, 3.f);
	// Draw lines around the title
	drawTitleLines(g, titleArea.toFloat());
	// Draw title text
	g.setColour(Colours::antiquewhite.brighter(.4f));
	g.setFont(Font("Pacifico", 50, Font::plain));
	g.drawFittedText("LCR Chorus", titleArea, Justification::centred, 1);

	// Fill LCR control area and border around
	Colour LCRColour = Colours::darkblue.darker(1).contrasting(.3f);
	g.setColour(boxColour);
	g.fillRoundedRectangle(area.toFloat(), 10.f);
	g.setColour(bgColour.darker(0.8f));
	g.drawRoundedRectangle(area.toFloat(), 10.f, 3.f);
	// Draw lines dividing controller sections
	area.removeFromTop(mTitleHeight);
	Line<float> myLine(area.getTopRight().toFloat(), area.getTopLeft().toFloat());
	g.setColour(boxColour.darker(.4));
	myLine = myLine.withShortenedEnd(4.f);
	myLine = myLine.withShortenedStart(4.f);
	g.drawLine(myLine, 3.f);
	for (auto i = 0; i < 2; ++i)
	{
		area.removeFromTop(mSliderHeight);
		myLine = Line<float>(area.getTopRight().toFloat(), area.getTopLeft().toFloat());
		myLine = myLine.withShortenedEnd(4.f);
		myLine = myLine.withShortenedStart(4.f);
		g.drawLine(myLine, 3.f);
	}
}

//==============================================================================
void ChorusAudioProcessorEditor::drawTitleLines(Graphics & g, Rectangle<float> titleArea)
{
	auto tlTitle = titleArea.getTopLeft();
	auto trTitle = titleArea.getTopRight();
	g.setColour(Colours::antiquewhite);
	// First
	g.drawLine(tlTitle.getX() + 4.f, tlTitle.getY() + 10.f, trTitle.getX() - 4.f, trTitle.getY() + 10.f, 2.f);
	// Second
	float lineNum = 2;
	g.drawLine(tlTitle.getX() + 4.f  , tlTitle.getY() + 10.f * lineNum, tlTitle.getX()  + 40.f, trTitle.getY() + 10.f * lineNum, 2.f);
	g.drawLine(tlTitle.getX() + 102.f, tlTitle.getY() + 10.f * lineNum, tlTitle.getX() + 111.f, trTitle.getY() + 10.f * lineNum, 2.f);
	g.drawLine(tlTitle.getX() + 145.f, tlTitle.getY() + 10.f * lineNum, trTitle.getX() - 4.f  , trTitle.getY() + 10.f * lineNum, 2.f);
	// Third
	lineNum = 3;
	g.drawLine(tlTitle.getX() + 4.f  , tlTitle.getY() + 10.f * lineNum, tlTitle.getX()  + 36.f, trTitle.getY() + 10.f * lineNum, 2.f);
	g.drawLine(tlTitle.getX() + 194.f, tlTitle.getY() + 10.f * lineNum, trTitle.getX()  - 4.f , trTitle.getY() + 10.f * lineNum, 2.f);
	// Fourth
	lineNum = 4;
	g.drawLine(tlTitle.getX() + 4.f  , tlTitle.getY() + 10.f * lineNum, tlTitle.getX() + 32.f, trTitle.getY() + 10.f * lineNum, 2.f);
	g.drawLine(tlTitle.getX() + 200.f, tlTitle.getY() + 10.f * lineNum, trTitle.getX() - 4.f , trTitle.getY() + 10.f * lineNum, 2.f);
	// Fifth
	lineNum = 5;
	g.drawLine(tlTitle.getX() + 4.f  , tlTitle.getY() + 10.f * lineNum, trTitle.getX() - 4.f, trTitle.getY() + 10.f * lineNum, 2.f);
}

//==============================================================================
void ChorusAudioProcessorEditor::drawLegs(Graphics& g, Rectangle<float> area)
{
	auto areaBottomLeft  = area.getBottomLeft();
	auto areaBottomRight = area.getBottomRight();
	auto distance = area.getWidth() * 0.15f;
	g.drawLine(areaBottomLeft.getX()  + distance, areaBottomLeft.getY(), areaBottomLeft.getX()  + distance, areaBottomLeft.getY() + 10.f, 10.f);
	g.drawLine(areaBottomRight.getX() - distance, areaBottomLeft.getY(), areaBottomRight.getX() - distance, areaBottomLeft.getY() + 10.f, 10.f);

}

//==============================================================================
void ChorusAudioProcessorEditor::resized()
{
	auto area = getLocalBounds().reduced(mWindowMarginWidth, mWindowMarginHeight);
	auto controlArea = area.removeFromTop(mLabelHeight + mSliderHeight + 10.f);
	controlArea.removeFromBottom(10.f);

	// Mix ===============================
	FlexBox mixBox;
	mixBox.alignContent = FlexBox::AlignContent::center;
	mixBox.justifyContent = FlexBox::JustifyContent::center;
	mixBox.flexDirection = FlexBox::Direction::column;
	mixBox.items.addArray(
		{
			FlexItem(mMixLabel) .withWidth(mLabelWidht) .withHeight(mLabelHeight),
			FlexItem(mMixSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});

	auto mixArea = controlArea.removeFromRight(mSliderWidth);
	mixBox.performLayout(mixArea.toFloat());

	//LCR box
	FlexBox LCRBox;
	LCRBox.alignContent = FlexBox::AlignContent::center;
	LCRBox.justifyContent = FlexBox::JustifyContent::center;
	LCRBox.flexDirection = FlexBox::Direction::row;
	LCRBox.items.addArray(
		{
			FlexItem(mLeftTitleLabel)  .withWidth(mSliderWidth).withHeight(mTitleHeight),
			FlexItem(mCenterTitleLabel).withWidth(mSliderWidth).withHeight(mTitleHeight),
			FlexItem(mRightTitleLabel) .withWidth(mSliderWidth).withHeight(mTitleHeight),
		});

	auto LCRArea = area.removeFromTop(mTitleHeight);
	LCRArea.removeFromLeft(mLabelWidht);
	LCRBox.performLayout(LCRArea);

	// Title box
	FlexBox titleBox;
	titleBox.alignContent = FlexBox::AlignContent::center;
	titleBox.justifyContent = FlexBox::JustifyContent::center;
	titleBox.flexDirection = FlexBox::Direction::column;
	titleBox.items.addArray(
		{
			FlexItem(mFreqTitleLabel)    .withWidth(mLabelWidht).withHeight(mSliderHeight),
			FlexItem(mDepthTitleLabel)   .withWidth(mLabelWidht).withHeight(mSliderHeight),
			FlexItem(mFeedbackTitleLabel).withWidth(mLabelWidht).withHeight(mSliderHeight)
		});
	auto titleArea = area.removeFromLeft(mLabelWidht);
	titleBox.performLayout(titleArea.toFloat());
	
	// Left box
	FlexBox leftBox;
	leftBox.alignContent = FlexBox::AlignContent::center;
	leftBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	leftBox.flexDirection = FlexBox::Direction::column;
	leftBox.items.addArray(
		{
			FlexItem(mLfoFreqLSlider) .withWidth(mSliderWidth).withHeight(mSliderHeight),
			FlexItem(mLfoDepthLSlider).withWidth(mSliderWidth).withHeight(mSliderHeight),
			FlexItem(mFeedbackLSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});

	auto leftArea = area.removeFromLeft(mSliderWidth);
	leftBox.performLayout(leftArea.toFloat());

	// Center box
	FlexBox centerBox;
	centerBox.alignContent = FlexBox::AlignContent::center;
	centerBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	centerBox.flexDirection = FlexBox::Direction::column;
	centerBox.items.addArray(
		{
			FlexItem(mLfoFreqCSlider) .withWidth(mSliderWidth).withHeight(mSliderHeight),
			FlexItem(mLfoDepthCSlider).withWidth(mSliderWidth).withHeight(mSliderHeight),
			FlexItem(mFeedbackCSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});
	auto centerArea = area.removeFromLeft(mSliderWidth);
	centerBox.performLayout(centerArea.toFloat());

	// Right box
	FlexBox rightBox;
	rightBox.alignContent = FlexBox::AlignContent::center;
	rightBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
	rightBox.flexDirection = FlexBox::Direction::column;
	rightBox.items.addArray(
		{
			FlexItem(mLfoFreqRSlider) .withWidth(mSliderWidth).withHeight(mSliderHeight),
			FlexItem(mLfoDepthRSlider).withWidth(mSliderWidth).withHeight(mSliderHeight),
			FlexItem(mFeedbackRSlider).withWidth(mSliderWidth).withHeight(mSliderHeight)
		});
	auto rightArea = area.removeFromLeft(mSliderWidth);
	rightBox.performLayout(rightArea.toFloat());
}

//==============================================================================
void ChorusAudioProcessorEditor::initialiseGUI()
{
	Colour labelColour = Colours::antiquewhite.brighter(.4f);
	// MIX =====================================
	// Label
	mMixLabel.setText("Mix", dontSendNotification);
	mMixLabel.setColour(Label::textColourId, labelColour);
	mMixLabel.setSize(mLabelWidht, mLabelHeight);
	mMixLabel.setFont(mLabelFont);
	mMixLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mMixLabel);
	// Slider
	mMixSlider.setSliderStyle(mSliderStyle);
	mMixSlider.setSize(mSliderWidth, mSliderHeight);
	mMixSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mMixSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mMixSlider);
	mMixSliderAttachment.reset(new SliderAttachment(mState, IDs::wetness, mMixSlider));
	mMixSlider.setLookAndFeel(&mKnobLookAndFeel);

	// LFO FREQUENCY ===========================
	// Left ===================
	// Slider
	mLfoFreqLSlider.setSliderStyle(mSliderStyle);
	mLfoFreqLSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoFreqLSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoFreqLSlider.setTextValueSuffix(" Hz");
	addAndMakeVisible(mLfoFreqLSlider);
	mLfoFreqLSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoFreqL, mLfoFreqLSlider));
	mLfoFreqLSlider.setLookAndFeel(&mKnobLookAndFeel);

	// Center ==================
	// Slider
	mLfoFreqCSlider.setSliderStyle(mSliderStyle);
	mLfoFreqCSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoFreqCSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoFreqCSlider.setTextValueSuffix(" Hz");
	addAndMakeVisible(mLfoFreqCSlider);
	mLfoFreqCSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoFreqC, mLfoFreqCSlider));
	mLfoFreqCSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Right ===================
	// Slider
	mLfoFreqRSlider.setSliderStyle(mSliderStyle);
	mLfoFreqRSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoFreqRSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoFreqRSlider.setTextValueSuffix(" Hz");
	addAndMakeVisible(mLfoFreqRSlider);
	mLfoFreqRSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoFreqR, mLfoFreqRSlider));
	mLfoFreqRSlider.setLookAndFeel(&mKnobLookAndFeel);

	// LFO DEPTH ===============================
	// Left ====================
	// Slider
	mLfoDepthLSlider.setSliderStyle(mSliderStyle);
	mLfoDepthLSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoDepthLSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoDepthLSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mLfoDepthLSlider);
	mLfoDepthLSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoDepthL, mLfoDepthLSlider));
	mLfoDepthLSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Center =================
	// Slider
	mLfoDepthCSlider.setSliderStyle(mSliderStyle);
	mLfoDepthCSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoDepthCSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoDepthCSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mLfoDepthCSlider);
	mLfoDepthCSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoDepthC, mLfoDepthCSlider));
	mLfoDepthCSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Right ===================
	// Slider
	mLfoDepthRSlider.setSliderStyle(mSliderStyle);
	mLfoDepthRSlider.setSize(mSliderWidth, mSliderHeight);
	mLfoDepthRSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mLfoDepthRSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mLfoDepthRSlider);
	mLfoDepthRSliderAttachment.reset(new SliderAttachment(mState, IDs::lfoDepthR, mLfoDepthRSlider));
	mLfoDepthRSlider.setLookAndFeel(&mKnobLookAndFeel);

	// FEEDBACK ================================
	// Left ====================
	// Slider
	mFeedbackLSlider.setSliderStyle(mSliderStyle);
	mFeedbackLSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackLSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackLSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackLSlider);
	mFeedbackLSliderAttachment.reset(new SliderAttachment(mState, IDs::feedbackL, mFeedbackLSlider));
	mFeedbackLSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Center ==================
	// Slider
	mFeedbackCSlider.setSliderStyle(mSliderStyle);
	mFeedbackCSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackCSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackCSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackCSlider);
	mFeedbackCSliderAttachment.reset(new SliderAttachment(mState, IDs::feedbackC, mFeedbackCSlider));
	mFeedbackCSlider.setLookAndFeel(&mKnobLookAndFeel);
	// Right ===================
	// Slider
	mFeedbackRSlider.setSliderStyle(mSliderStyle);
	mFeedbackRSlider.setSize(mSliderWidth, mSliderHeight);
	mFeedbackRSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	mFeedbackRSlider.setTextValueSuffix(" %");
	addAndMakeVisible(mFeedbackRSlider);
	mFeedbackRSliderAttachment.reset(new SliderAttachment(mState, IDs::feedbackR, mFeedbackRSlider));
	mFeedbackRSlider.setLookAndFeel(&mKnobLookAndFeel);

	// TITLE LABELS =====================================
	// FeedbackTitle
	mFeedbackTitleLabel.setText("Feedback", dontSendNotification);
	mFeedbackTitleLabel.setColour(Label::textColourId, labelColour);
	mFeedbackTitleLabel.setSize(mLabelWidht, mSliderHeight);
	mFeedbackTitleLabel.setFont(mLabelFont);
	mFeedbackTitleLabel.setJustificationType(Justification::centredRight);
	addAndMakeVisible(mFeedbackTitleLabel);
	// Depth
	mDepthTitleLabel.setText("Depth", dontSendNotification);
	mDepthTitleLabel.setColour(Label::textColourId, labelColour);
	mDepthTitleLabel.setSize(mLabelWidht, mSliderHeight);
	mDepthTitleLabel.setFont(mLabelFont);
	mDepthTitleLabel.setJustificationType(Justification::centredRight);
	addAndMakeVisible(mDepthTitleLabel);
	// Frequency
	mFreqTitleLabel.setText("Frequency", dontSendNotification);
	mFreqTitleLabel.setColour(Label::textColourId, labelColour);
	mFreqTitleLabel.setSize(mLabelWidht, mSliderHeight);
	mFreqTitleLabel.setFont(mLabelFont);
	mFreqTitleLabel.setJustificationType(Justification::centredRight);
	addAndMakeVisible(mFreqTitleLabel);
	// Left
	mLeftTitleLabel.setText("L", dontSendNotification);
	mLeftTitleLabel.setColour(Label::textColourId, labelColour);
	mLeftTitleLabel.setSize(mTitleWidth, mTitleHeight);
	mLeftTitleLabel.setFont(mTitleFont);
	mLeftTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mLeftTitleLabel);
	// Center
	mCenterTitleLabel.setText("C", dontSendNotification);
	mCenterTitleLabel.setColour(Label::textColourId, labelColour);
	mCenterTitleLabel.setSize(mTitleWidth, mTitleHeight);
	mCenterTitleLabel.setFont(mTitleFont);
	mCenterTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mCenterTitleLabel);
	// Right
	mRightTitleLabel.setText("R", dontSendNotification);
	mRightTitleLabel.setColour(Label::textColourId, labelColour);
	mRightTitleLabel.setSize(mTitleWidth, mTitleHeight);
	mRightTitleLabel.setFont(mTitleFont);
	mRightTitleLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(mRightTitleLabel);
}
