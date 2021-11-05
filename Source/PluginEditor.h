/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "RotarySlider.h"

//==============================================================================
/**
*/
class FreeDelayAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    FreeDelayAudioProcessorEditor (FreeDelayAudioProcessor&);
    ~FreeDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;


private:
    FreeDelayAudioProcessor& audioProcessor;
    juce::UndoManager& undoManager;

    RotarySlider blueSlider, yellowSlider, greenSlider;
    juce::Label title, delayTime, dryWet, feedback;
    juce::TextButton undo, redo;

    juce::Colour blue = juce::Colour::fromFloatRGBA(0.43f, 0.83f, 1.0f, 1.0f);
    juce::Colour green = juce::Colour::fromFloatRGBA(0.34f, 0.74f, 0.66f, 1.0f);
    juce::Colour yellow = juce::Colour::fromFloatRGBA(1.0f, 0.71f, 0.2f, 1.0f);
    juce::Colour black = juce::Colour::fromFloatRGBA(0.08f, 0.08f, 0.08f, 1.0f);

    juce::AudioProcessorValueTreeState::SliderAttachment blueSliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment yellowSliderAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment greenSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreeDelayAudioProcessorEditor)
};
