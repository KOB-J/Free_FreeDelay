/*
  ==============================================================================

    Delay.cpp
    Created: 5 Nov 2021 11:50:12am
    Author:  dev

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay(): delayBuffer_(2, 1)
{
    delayLength = 0.5;
    dryMix = 1.0;
    wetMix = 0.5;
    feedback = 0.75;
    delayBufferLength = 1;

    delayReadPosition = 0;
    delayWritePosition = 0;

    delaySampleRate = 0.0;
    delaySamplesPerBlock = 0;
}

Delay::~Delay()
{
}

void Delay::prepare(double& sampleRate, int& samplesPerBlock)
{
    delaySampleRate = sampleRate;
    delaySamplesPerBlock = samplesPerBlock;

    delayBufferLength = (int)(2.0 * sampleRate);
    if (delayBufferLength < 1)
        delayBufferLength = 1;
    delayBuffer_.setSize(2, delayBufferLength);
    delayBuffer_.clear();

    delayReadPosition = (int)(delayWritePosition - (delayLength * sampleRate)
        + delayBufferLength) % delayBufferLength;
}

void Delay::process(juce::AudioBuffer<float>& buffer, float& delayLengthParameter, float& dryWetParameter, float& feedbackParameter)
{
    dryMix = 1 - dryWetParameter;
    wetMix = dryWetParameter;
    delayLength = delayLengthParameter;
    feedback = feedbackParameter;

    delayReadPosition = (int)(delayWritePosition - (delayLength * delaySampleRate) + delayBufferLength) % delayBufferLength;

    const int numChannels = buffer.getNumChannels();    
    const int numSamples = buffer.getNumSamples();
    int channel, blockDelayReadPosition, blockDelayWritePosition;
    //TODO Interpolation
    for (channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        float* delayData = delayBuffer_.getWritePointer(juce::jmin(channel, delayBuffer_.getNumChannels() - 1));

        blockDelayReadPosition = delayReadPosition;
        blockDelayWritePosition = delayWritePosition;

        for (int i = 0; i < numSamples; ++i)
        {
            const float in = channelData[i];
            float out = 0.0;

            out = (dryMix * in + wetMix * delayData[blockDelayReadPosition]);

            delayData[blockDelayWritePosition] = in + (delayData[blockDelayReadPosition] * feedback);

            if (++blockDelayReadPosition >= delayBufferLength)
                blockDelayReadPosition = 0;
            if (++blockDelayWritePosition >= delayBufferLength)
                blockDelayWritePosition = 0;

            channelData[i] = out;
        }
    }

    delayReadPosition = blockDelayReadPosition;
    delayWritePosition = blockDelayWritePosition;
}

void Delay::reset()
{
    delayBuffer_.clear();
}
