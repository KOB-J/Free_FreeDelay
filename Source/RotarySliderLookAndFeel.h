/*
  ==============================================================================

    RotarySliderLookAndFeel.h
    Created: 5 Nov 2021 1:52:04pm
    Author:  dev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class RotarySliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    RotarySliderLookAndFeel();
    ~RotarySliderLookAndFeel();

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;

private:
};
