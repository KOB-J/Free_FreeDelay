/*
  ==============================================================================

    Delay.cpp
    Created: 5 Nov 2021 11:50:12am
    Author:  dev

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay(): delayBuffer(2, 1)
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

    delayLenghtSmoothed = delayLength;
    counter = 0;
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
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();

    delayReadPosition = (int)(delayWritePosition - (delayLength * sampleRate)
        + delayBufferLength) % delayBufferLength;
}

void Delay::process(juce::AudioBuffer<float>& buffer, float& delayLengthParameter, float& dryWetParameter, float& feedbackParameter)
{
    dryMix = 1.0f - dryWetParameter;
    wetMix = dryWetParameter;
    delayLength = delayLengthParameter;
    feedback = feedbackParameter;

    float entryDelayLenght = delayLength;

    const int numChannels = buffer.getNumChannels();    
    const int numSamples = buffer.getNumSamples();
    int channel = 0, blockDelayReadPosition, blockDelayWritePosition;

    for (channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));

        blockDelayReadPosition = delayReadPosition;
        blockDelayWritePosition = delayWritePosition;

        for (int i = 0; i < numSamples; ++i)
        {
            blockDelayReadPosition = (int)(blockDelayWritePosition - (int)(delayLenghtSmoothed * delaySampleRate) + delayBufferLength) % delayBufferLength;

            float delayReadPositionFloat = blockDelayWritePosition - (delayLenghtSmoothed * delaySampleRate);
            if (delayReadPositionFloat < 0) {
                delayReadPositionFloat += (float)delayBufferLength;
            }

            float interpolatedSample = delayData[blockDelayReadPosition];

            //******** Uncomment, if you want to use interpolation fonction    ***********************
            //int delayReadPosition_x = (int)(blockDelayWritePosition - (delayLenghtSmoothed * delaySampleRate) + delayBufferLength) % delayBufferLength;
            //int delayReadPosition_x1 = (int)(blockDelayWritePosition - (delayLenghtSmoothed * delaySampleRate) + 1 + delayBufferLength) % delayBufferLength;
            //float delayReadPositionInterleave = delayReadPositionFloat - delayReadPosition_x;
            //float interpolatedSample = linearInterpolation(delayData[delayReadPosition_x], delayData[delayReadPosition_x1], delayReadPositionInterleave);

            const float in = channelData[i];
            float out = 0.0;

            float centerGain = 1.2f;
            out = (dryMix + (dryMix * centerGain * juce::jmin<float>(dryMix, wetMix))) * in
                + (wetMix + (wetMix * centerGain * juce::jmin<float>(dryMix, wetMix))) * interpolatedSample;

            if (delayLenghtSmoothed != delayLength)
                interpolatedSample = 0.0f;

            delayData[blockDelayWritePosition] = in + (interpolatedSample * feedback);

            if (++blockDelayReadPosition >= delayBufferLength)
                blockDelayReadPosition = 0;
            if (++blockDelayWritePosition >= delayBufferLength)
                blockDelayWritePosition = 0;

            channelData[i] = out;
        }
    }

    if (entryDelayLenght == delayLength) counter++;

    if (counter == 60) {
        counter = 0;
        delayLenghtSmoothed = delayLength;
    }

    delayReadPosition = blockDelayReadPosition;
    delayWritePosition = blockDelayWritePosition;
}

void Delay::reset()
{
    delayBuffer.clear();
}

float Delay::linearInterpolation(float x, float x1, float interleave)
{
    return (1 - interleave) * x + interleave * x1;
}
