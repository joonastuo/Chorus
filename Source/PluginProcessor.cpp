/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusAudioProcessor::ChorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	mState(*this, nullptr, Identifier("VibraFlange"),
		{
			  std::make_unique<AudioParameterFloat>(IDs::volume,
													"Volume",
													NormalisableRange<float>(-12.f, 12.f),
												    0.0,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(value * 100.f) / 100.f); },
												    [](const String& text) {return text.getFloatValue(); }
													 ),
			  std::make_unique<AudioParameterFloat>(IDs::wetness,
													"Mix",
													NormalisableRange<float>(0.00, 1.0),
												    0.5,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(value * 100.f * 100.f) / 100.f); },
												    [](const String& text) {return text.getFloatValue() / 100.f; }
													 ),
			  std::make_unique<AudioParameterFloat>(IDs::feedbackL,
													"Feedback left",
													NormalisableRange<float>(-0.99, 0.99),
												    0.0,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(value * 100.f * 100.f) / 100.f); },
												    [](const String& text) {return text.getFloatValue() / 100.f; }
													),
			  std::make_unique<AudioParameterFloat>(IDs::feedbackC,
													"Feedback center",
													NormalisableRange<float>(-0.99, 0.99),
												    0.0,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(value * 100.f * 100.f) / 100.f); },
												    [](const String& text) {return text.getFloatValue() / 100.f; }
													),
			  std::make_unique<AudioParameterFloat>(IDs::feedbackR,
													"Feedback right",
													NormalisableRange<float>(-0.99, 0.99),
												    0.0,
												    String(),
												    AudioProcessorParameter::genericParameter,
												    [](float value, int maxStringLength) {return static_cast<String>(round(value * 100.f * 100.f) / 100.f); },
												    [](const String& text) {return text.getFloatValue() / 100.f; }
													),
			 std::make_unique<AudioParameterFloat>(IDs::lfoFreqL,
													 "LFO Frequency left",
													 NormalisableRange<float>(0.0, 100.0),
													 40.0,
													 String(),
													 AudioProcessorParameter::genericParameter,
												     [](float value, int maxStringLength) {return static_cast<String>(round(0.02f * exp(0.05521f * value) * 100.f) / 100.f); },
												     [](const String& text) {return log(50 * text.getFloatValue()) / 0.05521; }),
			 std::make_unique<AudioParameterFloat>(IDs::lfoFreqC,
													 "LFO Frequency center",
													 NormalisableRange<float>(0.0, 100.0),
													 40.0,
													 String(),
													 AudioProcessorParameter::genericParameter,
												     [](float value, int maxStringLength) {return static_cast<String>(round(0.02f * exp(0.05521f * value) * 100.f) / 100.f); },
												     [](const String& text) {return log(50 * text.getFloatValue()) / 0.05521; }),
			 std::make_unique<AudioParameterFloat>(IDs::lfoFreqR,
													 "LFO Frequency right",
													 NormalisableRange<float>(0.0, 100.0),
													 40.0,
													 String(),
													 AudioProcessorParameter::genericParameter,
												     [](float value, int maxStringLength) {return static_cast<String>(round(0.02f * exp(0.05521f * value) * 100.f) / 100.f); },
												     [](const String& text) {return log(50 * text.getFloatValue()) / 0.05521; }),
			 std::make_unique<AudioParameterFloat>(IDs::lfoDepthL,
													 "LFO depth left",
													 0.00,
													 100.0,
													 20.0),
			 std::make_unique<AudioParameterFloat>(IDs::lfoDepthC,
													 "LFO depth center",
													 0.00,
													 100.0,
													 20.0),
			 std::make_unique<AudioParameterFloat>(IDs::lfoDepthR,
													 "LFO depth right",
													 0.00,
													 100.0,
													 20.0),
			 std::make_unique<AudioParameterBool>(IDs::onOff,
													 "On/Off",
													 true)
		}),
	mChorus(mState)
#endif
{
}

ChorusAudioProcessor::~ChorusAudioProcessor()
{
}

//==============================================================================
const String ChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusAudioProcessor::setCurrentProgram (int index)
{
}

const String ChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	mChorus.prepare(sampleRate, samplesPerBlock, getNumOutputChannels());
}

void ChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ChorusAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	mChorus.process(buffer);
}

//==============================================================================
bool ChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChorusAudioProcessor::createEditor()
{
    return new ChorusAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	auto state = mState.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void ChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mState.state.getType()))
			mState.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState & ChorusAudioProcessor::getState()
{
	return mState;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusAudioProcessor();
}
