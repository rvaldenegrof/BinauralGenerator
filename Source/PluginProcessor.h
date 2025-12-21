#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "BinauralGenerator.h"

//==============================================================================
/**
    Plugin processor for Binaural Generator
*/
class BinauralAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    BinauralAudioProcessor();
    ~BinauralAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter access
    juce::AudioProcessorValueTreeState& getValueTreeState() { return parameters; }

    // Parameter IDs
    static constexpr const char* BASE_FREQUENCY_ID = "baseFrequency";
    static constexpr const char* BINAURAL_OFFSET_ID = "binauralOffset";
    static constexpr const char* LEFT_VOLUME_ID = "leftVolume";
    static constexpr const char* RIGHT_VOLUME_ID = "rightVolume";
    static constexpr const char* MASTER_VOLUME_ID = "masterVolume";
    static constexpr const char* MODE_ID = "mode";
    static constexpr const char* MUTE_ID = "mute";

    // Preset management
    void applyPreset (int presetIndex);
    
    // Export functionality (for standalone)
    enum class ExportFormat { WAV, MP3 };
    bool exportAudio (const juce::File& file, int presetIndex, double durationSeconds, 
                      ExportFormat format = ExportFormat::WAV, int mp3Bitrate = 192, 
                      double sampleRate = 44100.0);
    
private:
    // Helper for MP3 quality index
    int getMP3QualityIndex (int bitrate) const;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    
    // Binaural generator
    BinauralGenerator binauralGenerator;
    
    // Sample rate
    double currentSampleRate = 44100.0;
    
    // Parameter creation helper
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralAudioProcessor)
};

