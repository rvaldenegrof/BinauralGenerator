#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <functional>

class AndroidAudioEngine : private juce::AudioIODeviceCallback
{
public:
    AndroidAudioEngine();
    ~AndroidAudioEngine() override;

    bool initialize();
    void shutdown();
    bool start();
    void stop();
    bool isRunning() const noexcept;

    void setRenderCallback (std::function<void(juce::AudioBuffer<float>&)> callback);

private:
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples) override;

    void audioDeviceAboutToStart (juce::AudioIODevice* device) override;
    void audioDeviceStopped() override;

    juce::AudioDeviceManager deviceManager;
    std::function<void(juce::AudioBuffer<float>&)> renderCallback;
    bool running = false;
};
