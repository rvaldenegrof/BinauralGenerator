#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

//==============================================================================
/**
    Editor component for Binaural Generator plugin
*/
class BinauralAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit BinauralAudioProcessorEditor (BinauralAudioProcessor&);
    ~BinauralAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // Reference to processor
    BinauralAudioProcessor& audioProcessor;

    // UI Components
    juce::Slider baseFrequencySlider;
    juce::Label baseFrequencyLabel;
    juce::Slider binauralOffsetSlider;
    juce::Label binauralOffsetLabel;
    juce::Slider leftVolumeSlider;
    juce::Label leftVolumeLabel;
    juce::Slider rightVolumeSlider;
    juce::Label rightVolumeLabel;
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    juce::ToggleButton modeToggle;
    juce::Label modeLabel;
    juce::ToggleButton muteButton;
    juce::ComboBox presetComboBox;
    juce::Label presetLabel;
    
    // Export controls (only visible in standalone)
    juce::TextButton exportButton;
    juce::Slider durationSlider;
    juce::Label durationLabel;
    juce::Label durationDisplayLabel; // Shows time in MM:SS format
    juce::ComboBox formatComboBox;
    juce::Label formatLabel;
    juce::ComboBox mp3BitrateComboBox;
    juce::Label mp3BitrateLabel;
    juce::Label exportSectionLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> baseFrequencyAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> binauralOffsetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> modeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteAttachment;

    // Helper methods
    void setupSlider (juce::Slider& slider, juce::Label& label, const juce::String& labelText);
    void setupToggle (juce::ToggleButton& toggle, juce::Label& label, const juce::String& labelText);
    void setupMuteButton (juce::ToggleButton& button);
    void setupComboBox (juce::ComboBox& comboBox, juce::Label& label, const juce::String& labelText);
    
    // Callback for preset selection
    void presetComboBoxChanged();
    
    // Export functionality
    void setupExportControls();
    void exportButtonClicked();
    void startExport();
    void updateDurationDisplay();
    juce::String formatTime (double seconds);
    void updateFormatControls();
    void updateExportButtonText();
    
    // File chooser (needs to persist)
    std::unique_ptr<juce::FileChooser> fileChooser;
    
    // Export progress dialog
    std::unique_ptr<juce::AlertWindow> progressWindow;
    std::unique_ptr<juce::ProgressBar> progressBar;
    double currentProgress = 0.0;
    
    // Export thread
    class ExportThread : public juce::Thread
    {
    public:
        ExportThread (BinauralAudioProcessor& proc, const juce::File& f, int presetIdx,
                     double duration, BinauralAudioProcessor::ExportFormat fmt, int bitrate,
                     std::function<void(double)> progressCallback,
                     std::function<void(bool)> completionCallback)
            : Thread ("ExportThread"),
              processor (proc),
              file (f),
              presetIndex (presetIdx),
              durationSeconds (duration),
              format (fmt),
              mp3Bitrate (bitrate),
              onProgress (progressCallback),
              onComplete (completionCallback)
        {
        }
        
        void run() override
        {
            bool success = false;
            const int totalSteps = 100;
            
            // Simulate progress during export
            // Note: Actual export doesn't support progress callbacks, so we estimate
            for (int i = 0; i <= totalSteps && ! threadShouldExit(); ++i)
            {
                if (i == 0)
                {
                    // Start export
                    success = processor.exportAudio (file, presetIndex, durationSeconds, format, mp3Bitrate);
                }
                
                // Update progress (estimate based on time)
                double progress = i / static_cast<double> (totalSteps);
                if (onProgress)
                    onProgress (progress);
                
                // Small delay to allow UI updates
                sleep (10);
            }
            
            if (onComplete)
                onComplete (success);
        }
        
    private:
        BinauralAudioProcessor& processor;
        juce::File file;
        int presetIndex;
        double durationSeconds;
        BinauralAudioProcessor::ExportFormat format;
        int mp3Bitrate;
        std::function<void(double)> onProgress;
        std::function<void(bool)> onComplete;
    };
    
    std::unique_ptr<ExportThread> exportThread;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralAudioProcessorEditor)
};

