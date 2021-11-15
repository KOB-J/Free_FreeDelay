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
    float linearInterpolation(float x, float x1, float interleave);

private:
    juce::AudioSampleBuffer delayBuffer;
    int delayBufferLength;
    int delayReadPosition;
    int delayWritePosition;
    float delayLenghtSmoothed;
    int counter;

    float delayLength; 
    float dryMix;      
    float wetMix;      
    float feedback;    

    double delaySampleRate;
    int delaySamplesPerBlock;
};