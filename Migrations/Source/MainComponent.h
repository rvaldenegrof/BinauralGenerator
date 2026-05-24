#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "AndroidAudioEngine.h"
#include "AndroidTimerController.h"
#include "BinauralGeneratorWrapper.h"

class MainComponent : public juce::Component,
                      private juce::Button::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void buttonClicked (juce::Button* button) override;
    void updateTimerDisplay (int secondsRemaining);
    void updatePlayState (bool isPlaying);

    juce::TextButton playButton {"Play"};
    juce::TextButton stopButton {"Stop"};
    juce::Label timerLabel {"timerLabel", "00:00"};

    juce::Slider baseFrequencySlider;
    juce::Label baseFrequencyLabel {"baseFrequencyLabel", "Base Frequency"};

    juce::Slider binauralOffsetSlider;
    juce::Label binauralOffsetLabel {"binauralOffsetLabel", "Binaural Offset"};

    juce::Slider leftVolumeSlider;
    juce::Label leftVolumeLabel {"leftVolumeLabel", "Left Volume"};

    juce::Slider rightVolumeSlider;
    juce::Label rightVolumeLabel {"rightVolumeLabel", "Right Volume"};

    juce::ComboBox presetComboBox;
    juce::Label presetLabel {"presetLabel", "Preset"};

    AndroidAudioEngine audioEngine;
    AndroidTimerController timerController;
    BinauralGeneratorWrapper audioGenerator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
