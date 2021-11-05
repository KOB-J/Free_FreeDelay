/*
  ==============================================================================

    RotarySlider.h
    Created: 5 Nov 2021 1:51:39pm
    Author:  dev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RotarySliderLookAndFeel.h"

class RotarySlider : public juce::Slider
{
public:
    RotarySlider();
    ~RotarySlider();

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

private:
    RotarySliderLookAndFeel RotarySliderLookAndFeel;

    juce::Colour grey = juce::Colour::fromFloatRGBA(0.42f, 0.42f, 0.42f, 1.0f);
    juce::Colour blackGrey = juce::Colour::fromFloatRGBA(0.2f, 0.2f, 0.2f, 1.0f);
};
