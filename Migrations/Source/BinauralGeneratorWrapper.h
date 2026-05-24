#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "BinauralGenerator.h"

class BinauralGeneratorWrapper
{
public:
    BinauralGeneratorWrapper();
    void prepare (double sampleRate, int samplesPerBlock);
    void setBaseFrequency (float value);
    void setBinauralOffset (float value);
    void setLeftVolume (float value);
    void setRightVolume (float value);
    void setMasterVolume (float value);
    void setModeBinaural (bool shouldUseBinaural);
    void render (juce::AudioBuffer<float>& buffer);

private:
    BinauralGenerator generator;
    float currentSampleRate = 44100.0;
};
