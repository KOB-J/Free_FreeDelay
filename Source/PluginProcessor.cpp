/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FreeDelayAudioProcessor::FreeDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, &undoManager, "parameter", getParameterLayout())
#endif
{
}

FreeDelayAudioProcessor::~FreeDelayAudioProcessor()
{
}

//==============================================================================
const juce::String FreeDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FreeDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FreeDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FreeDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FreeDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FreeDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FreeDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FreeDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FreeDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void FreeDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FreeDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delay.prepare(sampleRate, samplesPerBlock);
}

void FreeDelayAudioProcessor::releaseResources()
{
    delay.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FreeDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FreeDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto delayLength = apvts.getRawParameterValue("delayLenght")->load();
    auto dryWet = apvts.getRawParameterValue("dryWet")->load();
    auto feedback = apvts.getRawParameterValue("feedback")->load();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    delay.process(buffer, delayLength, dryWet, feedback);
}

//==============================================================================
bool FreeDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FreeDelayAudioProcessor::createEditor()
{
    return new FreeDelayAudioProcessorEditor (*this);
}

//==============================================================================
void FreeDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FreeDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout FreeDelayAudioProcessor::getParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "delayLenght",
        "delayLenght",
        0.0f,
        1.0f,
        0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "dryWet",
        "dryWet",
        0.0f,
        1.0f,
        0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "feedback",
        "feedback",
        0.0f,
        1.0f,
        0.5f));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FreeDelayAudioProcessor();
}
