#pragma once

#include <juce_dsp/juce_dsp.h>

//==============================================================================
/**
    Simple sine wave oscillator for binaural generation
*/
class BinauralOscillator
{
public:
    BinauralOscillator()
    {
        oscillator.initialise ([] (float x) { return std::sin (x); });
    }

    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        oscillator.prepare (spec);
    }

    void reset()
    {
        oscillator.reset();
    }

    void setFrequency (float frequencyHz)
    {
        if (frequencyHz > 0.0f && frequencyHz <= sampleRate * 0.5f)
            oscillator.setFrequency (frequencyHz);
    }

    void setAmplitude (float amplitude)
    {
        gain.setGainLinear (amplitude);
    }

    template <typename ProcessContext>
    void process (const ProcessContext& context)
    {
        // Clear output first
        context.getOutputBlock().clear();
        
        // Generate oscillator signal (adds to output)
        oscillator.process (context);

        // Apply gain
        gain.process (context);
    }

private:
    juce::dsp::Oscillator<float> oscillator;
    juce::dsp::Gain<float> gain;
    double sampleRate = 44100.0;
};

