#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible (playButton);
    addAndMakeVisible (stopButton);
    addAndMakeVisible (timerLabel);

    addAndMakeVisible (baseFrequencySlider);
    addAndMakeVisible (baseFrequencyLabel);
    addAndMakeVisible (binauralOffsetSlider);
    addAndMakeVisible (binauralOffsetLabel);
    addAndMakeVisible (leftVolumeSlider);
    addAndMakeVisible (leftVolumeLabel);
    addAndMakeVisible (rightVolumeSlider);
    addAndMakeVisible (rightVolumeLabel);
    addAndMakeVisible (presetComboBox);
    addAndMakeVisible (presetLabel);

    playButton.addListener (this);
    stopButton.addListener (this);

    baseFrequencySlider.setRange (20.0, 20000.0, 0.1);
    binauralOffsetSlider.setRange (0.0, 100.0, 0.1);
    leftVolumeSlider.setRange (-60.0, 0.0, 0.1);
    rightVolumeSlider.setRange (-60.0, 0.0, 0.1);

    timerController.onTick = [this] (int remainingSeconds) { updateTimerDisplay (remainingSeconds); };
    timerController.onFinish = [this] () { updatePlayState (false); };

    // Aquí se pueden cargar presets y enlazar controles con audioGenerator.

    setSize (360, 640);
}

MainComponent::~MainComponent() = default;

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void MainComponent::resized()
{
    const int margin = 16;
    const int controlHeight = 48;
    int y = margin;

    playButton.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;
    stopButton.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;
    timerLabel.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;

    baseFrequencyLabel.setBounds (margin, y, getWidth() - 2 * margin, 24);
    y += 24;
    baseFrequencySlider.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;

    binauralOffsetLabel.setBounds (margin, y, getWidth() - 2 * margin, 24);
    y += 24;
    binauralOffsetSlider.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;

    leftVolumeLabel.setBounds (margin, y, getWidth() - 2 * margin, 24);
    y += 24;
    leftVolumeSlider.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;

    rightVolumeLabel.setBounds (margin, y, getWidth() - 2 * margin, 24);
    y += 24;
    rightVolumeSlider.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
    y += controlHeight + margin;

    presetLabel.setBounds (margin, y, getWidth() - 2 * margin, 24);
    y += 24;
    presetComboBox.setBounds (margin, y, getWidth() - 2 * margin, controlHeight);
}

void MainComponent::buttonClicked (juce::Button* button)
{
    if (button == &playButton)
    {
        audioEngine.start();
        timerController.start (15 * 60); // ejemplo: 15 minutos
        updatePlayState (true);
    }
    else if (button == &stopButton)
    {
        timerController.stop();
        audioEngine.stop();
        updatePlayState (false);
    }
}

void MainComponent::updateTimerDisplay (int secondsRemaining)
{
    const int minutes = secondsRemaining / 60;
    const int seconds = secondsRemaining % 60;
    timerLabel.setText (juce::String::formatted ("%02d:%02d", minutes, seconds), juce::dontSendNotification);
}

void MainComponent::updatePlayState (bool isPlaying)
{
    playButton.setEnabled (! isPlaying);
    stopButton.setEnabled (isPlaying);
}
