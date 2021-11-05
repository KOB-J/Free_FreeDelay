/*
  ==============================================================================

    Delay.h
    Created: 5 Nov 2021 11:50:12am
    Author:  dev

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Delay
{
public:
    Delay();
    ~Delay();

    void prepare (double& sampleRate, int& samplesPerBlock);
    void process (
        juce::AudioBuffer<float>& buffer,
        float& delayLengthParameter,
        float& dryWetParameter,
        float& feedbackParameter);
    void reset();

private:

    juce::AudioSampleBuffer delayBuffer_;
    int delayBufferLength;
    int delayReadPosition, delayWritePosition;

    float delayLength; 
    float dryMix;      
    float wetMix;      
    float feedback;    

    double delaySampleRate;
    int delaySamplesPerBlock;
};