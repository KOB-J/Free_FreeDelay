/*
  ==============================================================================

    RotarySliderLookAndFeel.cpp
    Created: 5 Nov 2021 1:52:04pm
    Author:  dev

  ==============================================================================
*/

#include "RotarySliderLookAndFeel.h"

RotarySliderLookAndFeel::RotarySliderLookAndFeel() {};
RotarySliderLookAndFeel::~RotarySliderLookAndFeel() {};

juce::Slider::SliderLayout RotarySliderLookAndFeel::getSliderLayout(juce::Slider& slider)
{
    auto localBounds = slider.getLocalBounds();

    juce::Slider::SliderLayout layout;

    layout.textBoxBounds = localBounds;
    layout.sliderBounds = localBounds;

    return layout;
}

void RotarySliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(2.0f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.085f;
    auto arcRadius = radius - lineW * 1.6f;

    juce::Path valueArc;
    valueArc.addCentredArc(bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        toAngle,
        true);

    g.setColour(fill);
    g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto thumbWidth = lineW * 2.0f;

    juce::Path thumb;
    thumb.addRectangle(-thumbWidth / 2, -thumbWidth / 2, thumbWidth, radius + lineW);

    g.setColour(fill);

    g.fillPath(thumb, juce::AffineTransform::rotation(toAngle + 3.12f).translated(bounds.getCentre()));

    g.fillEllipse(bounds.reduced(radius * 0.28));
}

juce::Label* RotarySliderLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    auto* label = new juce::Label();

    label->setFont(17.0f);
    label->setJustificationType(juce::Justification::centred);
    label->setColour(juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    label->setColour(juce::Label::textWhenEditingColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    label->setColour(juce::Label::outlineWhenEditingColourId, slider.findColour(juce::Slider::textBoxOutlineColourId));
    label->setInterceptsMouseClicks(false, false);

    return label;
}
