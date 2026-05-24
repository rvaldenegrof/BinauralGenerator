#include "AndroidAudioEngine.h"

AndroidAudioEngine::AndroidAudioEngine()
{
}

AndroidAudioEngine::~AndroidAudioEngine()
{
    stop();
    shutdown();
}

bool AndroidAudioEngine::initialize()
{
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    auto result = deviceManager.initialiseWithDefaultDevices (0, 2);
    return result.isEmpty();
}

void AndroidAudioEngine::shutdown()
{
    deviceManager.removeAudioCallback (this);
    deviceManager.closeAudioDevice();
    running = false;
}

bool AndroidAudioEngine::start()
{
    if (! running)
    {
        deviceManager.addAudioCallback (this);
        running = true;
    }
    return running;
}

void AndroidAudioEngine::stop()
{
    if (running)
    {
        deviceManager.removeAudioCallback (this);
        running = false;
    }
}

bool AndroidAudioEngine::isRunning() const noexcept
{
    return running;
}

void AndroidAudioEngine::setRenderCallback (std::function<void(juce::AudioBuffer<float>&)> callback)
{
    renderCallback = std::move(callback);
}

void AndroidAudioEngine::audioDeviceIOCallback (const float** inputChannelData,
                                                int numInputChannels,
                                                float** outputChannelData,
                                                int numOutputChannels,
                                                int numSamples)
{
    juce::AudioBuffer<float> buffer (outputChannelData, numOutputChannels, numSamples);
    buffer.clear();

    if (renderCallback)
        renderCallback (buffer);
}

void AndroidAudioEngine::audioDeviceAboutToStart (juce::AudioIODevice* device)
{
    juce::ignoreUnused (device);
}

void AndroidAudioEngine::audioDeviceStopped()
{
}
