/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FreeDelayAudioProcessorEditor::FreeDelayAudioProcessorEditor (FreeDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    undoManager(*p.getUndoManager()),
    blueSliderAttachment(*p.getApvts(), "delayLenght", blueSlider),
    greenSliderAttachment(*p.getApvts(), "dryWet", greenSlider),
    yellowSliderAttachment(*p.getApvts(), "feedback", yellowSlider)
{
    setSize (600, 200);

    blueSlider.setColour(juce::Slider::rotarySliderFillColourId, blue);
    greenSlider.setColour(juce::Slider::rotarySliderFillColourId, green);
    yellowSlider.setColour(juce::Slider::rotarySliderFillColourId, yellow);

    undo.setButtonText("Un");
    redo.setButtonText("Re");
    undo.setColour(juce::TextButton::buttonColourId, blue);
    redo.setColour(juce::TextButton::buttonColourId, yellow);
    undo.onClick = [this] {undoManager.undo(); };
    redo.onClick = [this] {undoManager.redo(); };


    title.setText("DELAY", juce::dontSendNotification);
    auto titleGradiantColour = juce::ColourGradient(blue, 50, 75, green, 100, 125, true);

    title.setColour(juce::Label::textColourId, blue);
    title.setFont(juce::Font(32.0f, juce::Font::bold));

    delayTime.setText("Time", juce::dontSendNotification);
    dryWet.setText("Dry/ Wet", juce::dontSendNotification);
    feedback.setText("Feedback", juce::dontSendNotification);

    addAndMakeVisible(blueSlider);
    addAndMakeVisible(greenSlider);
    addAndMakeVisible(yellowSlider);

    addAndMakeVisible(title);
    /*addAndMakeVisible(delayTime);
    addAndMakeVisible(dryWet);
    addAndMakeVisible(feedback);*/

    addAndMakeVisible(undo);
    addAndMakeVisible(redo);


    setWantsKeyboardFocus(true);

    //TODO remove if no needs
   // startTimer(50);
}

FreeDelayAudioProcessorEditor::~FreeDelayAudioProcessorEditor()
{
}

//==============================================================================
void FreeDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(black);
}

void FreeDelayAudioProcessorEditor::resized()
{
    title.setBounds(10, 10, 150, 40);
    delayTime.setBounds(120, 80, 80, 180);
    dryWet.setBounds(260, 80, 80, 180);
    feedback.setBounds(400, 80, 80, 180);
    //TODO gradient colour on the title
    //TODO center param labels

    blueSlider.setBounds(120, 80, 80, 80);
    greenSlider.setBounds(260, 80, 80, 80);
    yellowSlider.setBounds(400, 80, 80, 80);

    auto buttonSize = getWidth() * 0.1f;
    undo.setBounds(getWidth() - buttonSize * 2, 0, buttonSize, buttonSize / 3);
    redo.setBounds(getWidth() - buttonSize, 0, buttonSize, buttonSize / 3);
}

void FreeDelayAudioProcessorEditor::timerCallback()
{
    repaint();
}
