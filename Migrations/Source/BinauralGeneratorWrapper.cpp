#include "BinauralGeneratorWrapper.h"

BinauralGeneratorWrapper::BinauralGeneratorWrapper()
{
}

void BinauralGeneratorWrapper::prepare (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 };
    generator.prepare (spec);
}

void BinauralGeneratorWrapper::setBaseFrequency (float value)
{
    generator.setBaseFrequency (value);
}

void BinauralGeneratorWrapper::setBinauralOffset (float value)
{
    generator.setBinauralOffset (value);
}

void BinauralGeneratorWrapper::setLeftVolume (float value)
{
    generator.setLeftVolume (value);
}

void BinauralGeneratorWrapper::setRightVolume (float value)
{
    generator.setRightVolume (value);
}

void BinauralGeneratorWrapper::setMasterVolume (float value)
{
    generator.setMasterVolume (value);
}

void BinauralGeneratorWrapper::setModeBinaural (bool shouldUseBinaural)
{
    generator.setMode (shouldUseBinaural ? BinauralGenerator::Mode::Binaural
                                         : BinauralGenerator::Mode::Manual);
}

void BinauralGeneratorWrapper::render (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> audioBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> context (audioBlock);
    generator.process (context);
}
