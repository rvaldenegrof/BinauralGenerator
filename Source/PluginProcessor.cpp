#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Presets.h"
#include <juce_audio_formats/juce_audio_formats.h>

//==============================================================================
BinauralAudioProcessor::BinauralAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
     parameters (*this, nullptr, juce::Identifier ("BinauralGenerator"), createParameterLayout())
#endif
{
}

BinauralAudioProcessor::~BinauralAudioProcessor()
{
}

//==============================================================================
const juce::String BinauralAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BinauralAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BinauralAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BinauralAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BinauralAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BinauralAudioProcessor::getNumPrograms()
{
    return 1;
}

int BinauralAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BinauralAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String BinauralAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void BinauralAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void BinauralAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    binauralGenerator.prepare ({ sampleRate, (juce::uint32) samplesPerBlock, 2 });
}

void BinauralAudioProcessor::releaseResources()
{
    binauralGenerator.reset();
}

bool BinauralAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Support stereo output
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
  #endif
}

void BinauralAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (auto i = getTotalNumInputChannels(); i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Check if muted
    auto isMuted = parameters.getRawParameterValue (MUTE_ID)->load() > 0.5f;
    
    if (isMuted)
    {
        // Clear buffer if muted
        buffer.clear();
        return;
    }

    // Update parameters
    auto baseFreq = parameters.getRawParameterValue (BASE_FREQUENCY_ID)->load();
    auto offset = parameters.getRawParameterValue (BINAURAL_OFFSET_ID)->load();
    auto leftVol = parameters.getRawParameterValue (LEFT_VOLUME_ID)->load();
    auto rightVol = parameters.getRawParameterValue (RIGHT_VOLUME_ID)->load();
    auto masterVol = parameters.getRawParameterValue (MASTER_VOLUME_ID)->load();
    auto mode = parameters.getRawParameterValue (MODE_ID)->load() > 0.5f;

    // Update generator
    binauralGenerator.setBaseFrequency (baseFreq);
    binauralGenerator.setBinauralOffset (offset);
    binauralGenerator.setLeftVolume (juce::Decibels::decibelsToGain (leftVol));
    binauralGenerator.setRightVolume (juce::Decibels::decibelsToGain (rightVol));
    binauralGenerator.setMasterVolume (juce::Decibels::decibelsToGain (masterVol));
    binauralGenerator.setMode (mode ? BinauralGenerator::Mode::Binaural 
                                     : BinauralGenerator::Mode::Manual);

    // Process audio
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    binauralGenerator.process (context);
}

//==============================================================================
bool BinauralAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* BinauralAudioProcessor::createEditor()
{
    return new BinauralAudioProcessorEditor (*this);
}

//==============================================================================
void BinauralAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void BinauralAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout BinauralAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        BASE_FREQUENCY_ID,
        "Base Frequency",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 0.1f, 0.3f),
        440.0f,
        "Hz",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String (value, 1) + " Hz"; },
        [] (const juce::String& text) { return text.getFloatValue(); }));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        BINAURAL_OFFSET_ID,
        "Binaural Offset",
        juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f),
        10.0f,
        "Hz",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String (value, 1) + " Hz"; },
        [] (const juce::String& text) { return text.getFloatValue(); }));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        LEFT_VOLUME_ID,
        "Left Volume",
        juce::NormalisableRange<float> (-60.0f, 0.0f, 0.1f),
        -6.0f,
        "dB",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String (value, 1) + " dB"; },
        [] (const juce::String& text) { return text.getFloatValue(); }));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        RIGHT_VOLUME_ID,
        "Right Volume",
        juce::NormalisableRange<float> (-60.0f, 0.0f, 0.1f),
        -6.0f,
        "dB",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String (value, 1) + " dB"; },
        [] (const juce::String& text) { return text.getFloatValue(); }));

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        MASTER_VOLUME_ID,
        "Master Volume",
        juce::NormalisableRange<float> (-60.0f, 0.0f, 0.1f),
        0.0f,
        "dB",
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) { return juce::String (value, 1) + " dB"; },
        [] (const juce::String& text) { return text.getFloatValue(); }));

    params.push_back (std::make_unique<juce::AudioParameterBool>(
        MODE_ID,
        "Mode",
        true));

    params.push_back (std::make_unique<juce::AudioParameterBool>(
        MUTE_ID,
        "Mute",
        false));

    return { params.begin(), params.end() };
}

//==============================================================================
void BinauralAudioProcessor::applyPreset (int presetIndex)
{
    if (presetIndex < 0 || presetIndex >= BinauralPresets::NUM_PRESETS)
        return;

    const auto& preset = BinauralPresets::ALL_PRESETS[presetIndex];
    
    // Update parameters
    parameters.getParameter (BASE_FREQUENCY_ID)->setValueNotifyingHost (
        parameters.getParameterRange (BASE_FREQUENCY_ID).convertTo0to1 (preset.baseFrequency));
    
    parameters.getParameter (BINAURAL_OFFSET_ID)->setValueNotifyingHost (
        parameters.getParameterRange (BINAURAL_OFFSET_ID).convertTo0to1 (preset.offset));
    
    // Ensure mode is set to Binaural for presets
    parameters.getParameter (MODE_ID)->setValueNotifyingHost (1.0f);
}

//==============================================================================
bool BinauralAudioProcessor::exportAudio (const juce::File& file, int presetIndex, 
                                           double durationSeconds, ExportFormat format,
                                           int mp3Bitrate, double sampleRate)
{
    if (presetIndex < 0 || presetIndex >= BinauralPresets::NUM_PRESETS)
        return false;
    
    // Apply preset
    applyPreset (presetIndex);
    
    // Prepare processor for offline rendering
    const int blockSize = 512;
    prepareToPlay (sampleRate, blockSize);
    
    // Create temporary processor copy for rendering
    BinauralGenerator tempGenerator;
    tempGenerator.prepare ({ sampleRate, (juce::uint32) blockSize, 2 });
    
    // Get current parameter values
    auto baseFreq = parameters.getRawParameterValue (BASE_FREQUENCY_ID)->load();
    auto offset = parameters.getRawParameterValue (BINAURAL_OFFSET_ID)->load();
    auto leftVol = parameters.getRawParameterValue (LEFT_VOLUME_ID)->load();
    auto rightVol = parameters.getRawParameterValue (RIGHT_VOLUME_ID)->load();
    auto masterVol = parameters.getRawParameterValue (MASTER_VOLUME_ID)->load();
    
    tempGenerator.setBaseFrequency (baseFreq);
    tempGenerator.setBinauralOffset (offset);
    tempGenerator.setLeftVolume (juce::Decibels::decibelsToGain (leftVol));
    tempGenerator.setRightVolume (juce::Decibels::decibelsToGain (rightVol));
    tempGenerator.setMasterVolume (juce::Decibels::decibelsToGain (masterVol));
    tempGenerator.setMode (BinauralGenerator::Mode::Binaural);
    
    // Create audio format writer based on selected format
    std::unique_ptr<juce::OutputStream> fileStream (file.createOutputStream());
    if (fileStream == nullptr)
        return false;
    
    std::unique_ptr<juce::AudioFormatWriter> writer;
    using Opts = juce::AudioFormatWriterOptions;
    
    if (format == ExportFormat::WAV)
    {
        juce::WavAudioFormat wavFormat;
        writer.reset (wavFormat.createWriterFor (
            fileStream, Opts{}.withSampleRate (sampleRate)
                              .withNumChannels (2)
                              .withBitsPerSample (24)).release());
    }
    #if JUCE_USE_LAME_AUDIO_FORMAT
    else if (format == ExportFormat::MP3)
    {
        // Try to find LAME executable in common locations
        juce::File lameExecutable;
        
        #if JUCE_MAC
        // Common macOS locations
        lameExecutable = juce::File ("/usr/local/bin/lame");
        if (! lameExecutable.existsAsFile())
            lameExecutable = juce::File ("/opt/homebrew/bin/lame");
        if (! lameExecutable.existsAsFile())
            lameExecutable = juce::File::getSpecialLocation (juce::File::currentExecutableFile)
                                                              .getParentDirectory()
                                                              .getChildFile ("lame");
        #elif JUCE_WINDOWS
        // Common Windows locations
        lameExecutable = juce::File::getSpecialLocation (juce::File::currentExecutableFile)
                                                          .getParentDirectory()
                                                          .getChildFile ("lame.exe");
        if (! lameExecutable.existsAsFile())
            lameExecutable = juce::File ("C:\\Program Files\\LAME\\lame.exe");
        #elif JUCE_LINUX
        // Common Linux locations
        lameExecutable = juce::File ("/usr/bin/lame");
        if (! lameExecutable.existsAsFile())
            lameExecutable = juce::File ("/usr/local/bin/lame");
        #endif
        
        if (! lameExecutable.existsAsFile())
        {
            // LAME not found - return false
            return false;
        }
        
        juce::LAMEEncoderAudioFormat mp3Format (lameExecutable);
        
        // Create metadata with bitrate info (convert StringPairArray to unordered_map)
        std::unordered_map<juce::String, juce::String> metadata;
        metadata["id3title"] = "Binaural Generator Export";
        metadata["id3artist"] = "Binaural Generator";
        
        // Create writer with bitrate
        writer.reset (mp3Format.createWriterFor (
            fileStream, Opts{}.withSampleRate (sampleRate)
                              .withNumChannels (2)
                              .withBitsPerSample (16)
                              .withMetadataValues (metadata)
                              .withQualityOptionIndex (getMP3QualityIndex (mp3Bitrate))).release());
    }
    #endif
    
    if (writer == nullptr)
        return false;
    
    // Render audio
    juce::AudioBuffer<float> buffer (2, blockSize);
    juce::MidiBuffer midiBuffer;
    
    const int totalSamples = static_cast<int> (sampleRate * durationSeconds);
    int samplesRendered = 0;
    
    while (samplesRendered < totalSamples)
    {
        const int samplesToRender = juce::jmin (blockSize, totalSamples - samplesRendered);
        buffer.setSize (2, samplesToRender, false, false, true);
        buffer.clear();
        
        // Generate audio
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        tempGenerator.process (context);
        
        // Write to file
        if (! writer->writeFromAudioSampleBuffer (buffer, 0, samplesToRender))
        {
            writer.reset();
            return false;
        }
        
        samplesRendered += samplesToRender;
    }
    
    writer.reset();
    return true;
}

//==============================================================================
int BinauralAudioProcessor::getMP3QualityIndex (int bitrate) const
{
    // LAMEEncoderAudioFormat quality options:
    // 0-9: VBR quality levels
    // 10-23: CBR bitrates [32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320]
    // We use CBR for precise bitrate control
    
    const int cbrBitrates[] = { 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };
    const int baseIndex = 10; // CBR options start at index 10
    
    // Find closest CBR bitrate
    for (int i = 0; i < 14; ++i)
    {
        if (bitrate <= cbrBitrates[i])
            return baseIndex + i;
    }
    
    // Default to 320 kbps (highest)
    return baseIndex + 13;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BinauralAudioProcessor();
}

