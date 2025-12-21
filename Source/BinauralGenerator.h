#pragma once

#include <juce_dsp/juce_dsp.h>
#include "BinauralOscillator.h"

//==============================================================================
/**
    Main binaural generator class that manages two oscillators (left and right)
*/
class BinauralGenerator
{
public:
    enum class Mode
    {
        Manual,     // Independent frequency control for each channel
        Binaural    // Base frequency + offset for right channel
    };

    BinauralGenerator()
    {
    }

    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        leftOscillator.prepare (spec);
        rightOscillator.prepare (spec);
        masterGain.prepare (spec);
        processSpec = spec;
    }

    void reset()
    {
        leftOscillator.reset();
        rightOscillator.reset();
        masterGain.reset();
    }

    void setBaseFrequency (float frequencyHz)
    {
        baseFrequency = frequencyHz;
        updateFrequencies();
    }

    void setBinauralOffset (float offsetHz)
    {
        binauralOffset = offsetHz;
        updateFrequencies();
    }

    void setLeftFrequency (float frequencyHz)
    {
        leftFrequency = frequencyHz;
        if (mode == Mode::Manual)
            leftOscillator.setFrequency (leftFrequency);
    }

    void setRightFrequency (float frequencyHz)
    {
        rightFrequency = frequencyHz;
        if (mode == Mode::Manual)
            rightOscillator.setFrequency (rightFrequency);
    }

    void setLeftVolume (float amplitude)
    {
        leftOscillator.setAmplitude (amplitude);
    }

    void setRightVolume (float amplitude)
    {
        rightOscillator.setAmplitude (amplitude);
    }

    void setMasterVolume (float amplitude)
    {
        masterGain.setGainLinear (amplitude);
    }

    void setMode (Mode newMode)
    {
        mode = newMode;
        updateFrequencies();
    }

    template <typename ProcessContext>
    void process (const ProcessContext& context)
    {
        auto&& outputBlock = context.getOutputBlock();
        auto numChannels = outputBlock.getNumChannels();

        if (numChannels < 2)
            return;

        // Clear output
        outputBlock.clear();

        // Create temporary buffers for left and right channels
        juce::dsp::AudioBlock<float> leftBlock (outputBlock.getSingleChannelBlock (0));
        juce::dsp::AudioBlock<float> rightBlock (outputBlock.getSingleChannelBlock (1));

        // Generate left channel
        juce::dsp::ProcessContextReplacing<float> leftContext (leftBlock);
        leftOscillator.process (leftContext);

        // Generate right channel
        juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);
        rightOscillator.process (rightContext);

        // Apply master gain to entire output
        masterGain.process (context);
    }

private:
    void updateFrequencies()
    {
        if (mode == Mode::Binaural)
        {
            leftOscillator.setFrequency (baseFrequency);
            rightOscillator.setFrequency (baseFrequency + binauralOffset);
        }
        else
        {
            leftOscillator.setFrequency (leftFrequency);
            rightOscillator.setFrequency (rightFrequency);
        }
    }

    BinauralOscillator leftOscillator;
    BinauralOscillator rightOscillator;
    juce::dsp::Gain<float> masterGain;

    Mode mode = Mode::Binaural;
    float baseFrequency = 440.0f;
    float binauralOffset = 10.0f;
    float leftFrequency = 440.0f;
    float rightFrequency = 450.0f;

    juce::dsp::ProcessSpec processSpec;
};

