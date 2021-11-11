/*
  ==============================================================================

    RotarySlider.cpp
    Created: 5 Nov 2021 1:51:39pm
    Author:  dev

  ==============================================================================
*/

#include "RotarySlider.h"

RotarySlider::RotarySlider()
{
    setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
        juce::MathConstants<float>::pi * 2.75f,
        true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(0.5, 1, 0.09, false);
    setRange(0.0, 100.0, 0.01);
    setValue(50.0);
    setDoubleClickReturnValue(true, 50.0);
    setTextValueSuffix(" %");
    onValueChange = [&]()
    {
        if (getValue() < 10)
            setNumDecimalPlacesToDisplay(2);
        else if (10 <= getValue() && getValue() < 100)
            setNumDecimalPlacesToDisplay(1);
        else
            setNumDecimalPlacesToDisplay(0);
    };

    setColour(juce::Slider::textBoxTextColourId, blackGrey);
    setColour(juce::Slider::textBoxOutlineColourId, grey);
    setLookAndFeel(&RotarySliderLookAndFeel);
}

RotarySlider::~RotarySlider()
{
    setLookAndFeel(nullptr);
}

void RotarySlider::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);
}

void RotarySlider::mouseDown(const juce::MouseEvent& event)
{
    juce::Slider::mouseDown(event);
    setMouseCursor(juce::MouseCursor::NoCursor);
}

void RotarySlider::mouseUp(const juce::MouseEvent& event)
{
    juce::Slider::mouseUp(event);
    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition(event.source.getLastMouseDownPosition());
    setMouseCursor(juce::MouseCursor::NormalCursor);
}
