#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Presets.h"

//==============================================================================
BinauralAudioProcessorEditor::BinauralAudioProcessorEditor (BinauralAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Set editor size - calculated to fit all elements comfortably
    // Larger if standalone (for export controls)
    #if JucePlugin_Build_Standalone
    setSize (650, 1000);
    #else
    setSize (550, 600);
    #endif

    // Setup sliders and labels
    setupSlider (baseFrequencySlider, baseFrequencyLabel, "Base Frequency (Hz)");
    setupSlider (binauralOffsetSlider, binauralOffsetLabel, "Binaural Offset (Hz)");
    setupSlider (leftVolumeSlider, leftVolumeLabel, "Left Volume (dB)");
    setupSlider (rightVolumeSlider, rightVolumeLabel, "Right Volume (dB)");
    setupSlider (masterVolumeSlider, masterVolumeLabel, "Master Volume (dB)");
    setupToggle (modeToggle, modeLabel, "Mode");
    setupMuteButton (muteButton);
    setupComboBox (presetComboBox, presetLabel, "Preset");
    
    // Setup export controls (only in standalone)
    #if JucePlugin_Build_Standalone
    setupExportControls();
    #endif

    // Create parameter attachments
    baseFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::BASE_FREQUENCY_ID, baseFrequencySlider);
    
    binauralOffsetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::BINAURAL_OFFSET_ID, binauralOffsetSlider);
    
    leftVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::LEFT_VOLUME_ID, leftVolumeSlider);
    
    rightVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::RIGHT_VOLUME_ID, rightVolumeSlider);
    
    masterVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::MASTER_VOLUME_ID, masterVolumeSlider);
    
    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::MODE_ID, modeToggle);
    
    muteAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getValueTreeState(), BinauralAudioProcessor::MUTE_ID, muteButton);
}

BinauralAudioProcessorEditor::~BinauralAudioProcessorEditor()
{
}

//==============================================================================
void BinauralAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Draw title background
    auto titleArea = getLocalBounds().removeFromTop (55);
    g.setColour (juce::Colour (0xff2a2a2a));
    g.fillRect (titleArea);
    
    // Draw title
    g.setColour (juce::Colours::white);
    g.setFont (26.0f);
    g.drawFittedText ("Binaural Generator", titleArea.reduced (10, 5),
                      juce::Justification::centred, 1);

    // Draw section separator
    g.setColour (juce::Colour (0xff404040));
    g.drawLine (10, 55, getWidth() - 10, 55, 2.0f);
}

void BinauralAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    const int titleHeight = 60;
    area.removeFromTop (titleHeight); // Space for title and separator

    const int margin = 15;
    const int sliderHeight = 40;
    const int labelHeight = 18;
    const int spacing = 8;
    const int comboHeight = 26;
    const int buttonHeight = 32;
    
    int y = area.getY() + margin;

    // Preset ComboBox
    presetLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    presetComboBox.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, comboHeight);
    y += labelHeight + comboHeight + spacing + 2;

    // Mute Button
    muteButton.setBounds (margin, y, 130, buttonHeight);
    y += buttonHeight + spacing;

    // Base Frequency
    baseFrequencyLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    baseFrequencySlider.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, sliderHeight);
    y += labelHeight + sliderHeight + spacing + 2;

    // Binaural Offset
    binauralOffsetLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    binauralOffsetSlider.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, sliderHeight);
    y += labelHeight + sliderHeight + spacing + 2;

    // Mode Toggle
    modeLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    modeToggle.setBounds (margin, y + labelHeight + 2, 180, buttonHeight);
    y += labelHeight + buttonHeight + spacing + 2;

    // Left Volume
    leftVolumeLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    leftVolumeSlider.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, sliderHeight);
    y += labelHeight + sliderHeight + spacing + 2;

    // Right Volume
    rightVolumeLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    rightVolumeSlider.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, sliderHeight);
    y += labelHeight + sliderHeight + spacing + 2;

    // Master Volume
    masterVolumeLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    masterVolumeSlider.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, sliderHeight);
    y += labelHeight + sliderHeight + spacing + 2;
    
    // Export section (only in standalone)
    #if JucePlugin_Build_Standalone
    y += spacing * 2;
    
    // Export section label
    exportSectionLabel.setBounds (margin, y, getWidth() - 2 * margin, 20);
    y += 25;
    
    // Export preset selector
    exportPresetLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    exportPresetComboBox.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, comboHeight);
    y += labelHeight + comboHeight + spacing + 2;
    
    // Duration slider
    durationLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    durationSlider.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin - 100, sliderHeight);
    durationDisplayLabel.setBounds (getWidth() - margin - 90, y + labelHeight + 2, 90, sliderHeight);
    y += labelHeight + sliderHeight + spacing + 2;
    
    // Format selector
    formatLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
    formatComboBox.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, comboHeight);
    y += labelHeight + comboHeight + spacing + 2;
    
    // MP3 bitrate selector (conditionally visible)
    if (mp3BitrateComboBox.isVisible())
    {
        mp3BitrateLabel.setBounds (margin, y, getWidth() - 2 * margin, labelHeight);
        mp3BitrateComboBox.setBounds (margin, y + labelHeight + 2, getWidth() - 2 * margin, comboHeight);
        y += labelHeight + comboHeight + spacing + 2;
    }
    
    // Export button
    exportButton.setBounds (margin, y, getWidth() - 2 * margin, 38);
    y += 38 + spacing;
    #endif
     
    // Verify all elements fit
    #if JucePlugin_Build_Standalone
    if ((y + margin) > getHeight())
    {
        // If elements don't fit, increase window size
        setSize (getWidth(), y + margin + 20);
    }
    #else
    if ((y + margin) > getHeight())
    {
        setSize (getWidth(), y + margin + 20);
    }
    #endif
}

//==============================================================================
void BinauralAudioProcessorEditor::setupSlider (juce::Slider& slider, juce::Label& label,
                                                 const juce::String& labelText)
{
    addAndMakeVisible (slider);
    slider.setSliderStyle (juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);

    addAndMakeVisible (label);
    label.setText (labelText, juce::dontSendNotification);
    label.attachToComponent (&slider, false);
    label.setColour (juce::Label::textColourId, juce::Colours::white);
}

void BinauralAudioProcessorEditor::setupToggle (juce::ToggleButton& toggle, juce::Label& label,
                                                  const juce::String& labelText)
{
    addAndMakeVisible (toggle);
    toggle.setButtonText ("Binaural / Manual");

    addAndMakeVisible (label);
    label.setText (labelText, juce::dontSendNotification);
    label.attachToComponent (&toggle, false);
    label.setColour (juce::Label::textColourId, juce::Colours::white);
}

void BinauralAudioProcessorEditor::setupMuteButton (juce::ToggleButton& button)
{
    addAndMakeVisible (button);
    button.setButtonText ("MUTE");
    button.setColour (juce::ToggleButton::textColourId, juce::Colours::white);
    button.setColour (juce::ToggleButton::tickColourId, juce::Colours::red);
    button.setColour (juce::ToggleButton::tickDisabledColourId, juce::Colours::darkgrey);
}

void BinauralAudioProcessorEditor::setupComboBox (juce::ComboBox& comboBox, juce::Label& label,
                                                     const juce::String& labelText)
{
    addAndMakeVisible (comboBox);
    
    // Add presets to combo box
    comboBox.addItem ("Custom", 1);
    for (int i = 0; i < BinauralPresets::NUM_PRESETS; ++i)
    {
        const auto& preset = BinauralPresets::ALL_PRESETS[i];
        comboBox.addItem (preset.name + " - " + preset.description, i + 2);
    }
    comboBox.setSelectedId (1); // Default to Custom
    comboBox.onChange = [this] { presetComboBoxChanged(); };

    addAndMakeVisible (label);
    label.setText (labelText, juce::dontSendNotification);
    label.attachToComponent (&comboBox, false);
    label.setColour (juce::Label::textColourId, juce::Colours::white);
}

void BinauralAudioProcessorEditor::presetComboBoxChanged()
{
    int selectedId = presetComboBox.getSelectedId();
    
    if (selectedId == 1) // Custom
        return;
    
    // Apply preset (selectedId - 2 because Custom is 1, first preset is 2)
    int presetIndex = selectedId - 2;
    audioProcessor.applyPreset (presetIndex);
}

//==============================================================================
void BinauralAudioProcessorEditor::setupExportControls()
{
    // Export section label
    addAndMakeVisible (exportSectionLabel);
    exportSectionLabel.setText ("Export Audio", juce::dontSendNotification);
    exportSectionLabel.setFont (juce::FontOptions (18.0f));
    exportSectionLabel.setFont (exportSectionLabel.getFont().boldened());
    exportSectionLabel.setColour (juce::Label::textColourId, juce::Colours::lightblue);
    exportSectionLabel.setJustificationType (juce::Justification::centredLeft);
    
    // Export preset combo box
    addAndMakeVisible (exportPresetComboBox);
    for (int i = 0; i < BinauralPresets::NUM_PRESETS; ++i)
    {
        const auto& preset = BinauralPresets::ALL_PRESETS[i];
        exportPresetComboBox.addItem (preset.name + " - " + preset.description, i + 1);
    }
    exportPresetComboBox.setSelectedId (1); // Default to first preset
    
    addAndMakeVisible (exportPresetLabel);
    exportPresetLabel.setText ("Export Preset", juce::dontSendNotification);
    exportPresetLabel.attachToComponent (&exportPresetComboBox, false);
    exportPresetLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    
    // Duration slider (in minutes, 0-120)
    addAndMakeVisible (durationSlider);
    durationSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    durationSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 80, 20);
    durationSlider.setRange (0.0, 120.0, 0.1); // 0 to 120 minutes
    durationSlider.setValue (1.0); // Default 1 minute
    durationSlider.setTextValueSuffix (" min");
    durationSlider.onValueChange = [this] { updateDurationDisplay(); };
    
    addAndMakeVisible (durationLabel);
    durationLabel.setText ("Duration (minutes)", juce::dontSendNotification);
    durationLabel.attachToComponent (&durationSlider, false);
    durationLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    
    // Duration display label (shows MM:SS format)
    addAndMakeVisible (durationDisplayLabel);
    durationDisplayLabel.setText (formatTime (1.0 * 60.0), juce::dontSendNotification);
    durationDisplayLabel.setColour (juce::Label::textColourId, juce::Colours::lightblue);
    durationDisplayLabel.setJustificationType (juce::Justification::centred);
    
    // Format selector
    addAndMakeVisible (formatComboBox);
    formatComboBox.addItem ("WAV (24-bit)", 1);
    #if JUCE_USE_LAME_AUDIO_FORMAT
    formatComboBox.addItem ("MP3", 2);
    #endif
    formatComboBox.setSelectedId (1);
    formatComboBox.onChange = [this] { updateFormatControls(); };
    
    addAndMakeVisible (formatLabel);
    formatLabel.setText ("Format", juce::dontSendNotification);
    formatLabel.attachToComponent (&formatComboBox, false);
    formatLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    
    // MP3 bitrate selector (only visible when MP3 is selected)
    addAndMakeVisible (mp3BitrateComboBox);
    mp3BitrateComboBox.addItem ("128 kbps", 1);
    mp3BitrateComboBox.addItem ("192 kbps", 2);
    mp3BitrateComboBox.addItem ("256 kbps", 3);
    mp3BitrateComboBox.addItem ("320 kbps", 4);
    mp3BitrateComboBox.setSelectedId (2); // Default 192 kbps
    mp3BitrateComboBox.setVisible (false);
    
    addAndMakeVisible (mp3BitrateLabel);
    mp3BitrateLabel.setText ("MP3 Quality", juce::dontSendNotification);
    mp3BitrateLabel.attachToComponent (&mp3BitrateComboBox, false);
    mp3BitrateLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    mp3BitrateLabel.setVisible (false);
    
    // Export button
    addAndMakeVisible (exportButton);
    updateExportButtonText();
    exportButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff4a9eff));
    exportButton.setColour (juce::TextButton::textColourOffId, juce::Colours::white);
    exportButton.onClick = [this] { exportButtonClicked(); };
}

void BinauralAudioProcessorEditor::updateDurationDisplay()
{
    double minutes = durationSlider.getValue();
    double seconds = minutes * 60.0;
    durationDisplayLabel.setText (formatTime (seconds), juce::dontSendNotification);
}

juce::String BinauralAudioProcessorEditor::formatTime (double seconds)
{
    int totalSeconds = static_cast<int> (seconds);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int secs = totalSeconds % 60;
    
    if (hours > 0)
        return juce::String::formatted ("%d:%02d:%02d", hours, minutes, secs);
    else
        return juce::String::formatted ("%d:%02d", minutes, secs);
}

void BinauralAudioProcessorEditor::updateFormatControls()
{
    bool isMP3 = formatComboBox.getSelectedId() == 2;
    mp3BitrateComboBox.setVisible (isMP3);
    mp3BitrateLabel.setVisible (isMP3);
    updateExportButtonText();
    resized(); // Trigger layout update
}

void BinauralAudioProcessorEditor::updateExportButtonText()
{
    bool isMP3 = formatComboBox.getSelectedId() == 2;
    exportButton.setButtonText (isMP3 ? "Export to MP3..." : "Export to WAV...");
}

void BinauralAudioProcessorEditor::exportButtonClicked()
{
    startExport();
}

void BinauralAudioProcessorEditor::startExport()
{
    int selectedPresetId = exportPresetComboBox.getSelectedId();
    if (selectedPresetId < 1 || selectedPresetId > BinauralPresets::NUM_PRESETS)
        return;
    
    int presetIndex = selectedPresetId - 1; // ComboBox IDs start at 1
    double durationMinutes = durationSlider.getValue();
    double durationSeconds = durationMinutes * 60.0;
    
        // Determine format
        bool formatIsMP3 = formatComboBox.getSelectedId() == 2;
        auto format = formatIsMP3 ? BinauralAudioProcessor::ExportFormat::MP3 
                        : BinauralAudioProcessor::ExportFormat::WAV;
        
        // Get MP3 bitrate if applicable
        int mp3Bitrate = 192; // Default
        if (formatIsMP3)
        {
            int bitrateId = mp3BitrateComboBox.getSelectedId();
            if (bitrateId == 1) mp3Bitrate = 128;
            else if (bitrateId == 2) mp3Bitrate = 192;
            else if (bitrateId == 3) mp3Bitrate = 256;
            else if (bitrateId == 4) mp3Bitrate = 320;
        }
        
        // Create file chooser with appropriate extension
        juce::String extension = formatIsMP3 ? "*.mp3" : "*.wav";
        fileChooser = std::make_unique<juce::FileChooser> ("Save Binaural Audio As...",
                                                        juce::File::getSpecialLocation (juce::File::userDocumentsDirectory),
                                                        extension);
        
        auto flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
        
        fileChooser->launchAsync (flags, [this, presetIndex, durationSeconds, format, mp3Bitrate] (const juce::FileChooser& chooser)
        {
            auto file = chooser.getResult();
            
            if (file == juce::File())
                return; // User cancelled
            
            // Ensure correct extension
            bool fileIsMP3 = format == BinauralAudioProcessor::ExportFormat::MP3;
            if (fileIsMP3 && ! file.hasFileExtension ("mp3"))
                file = file.withFileExtension ("mp3");
            else if (! fileIsMP3 && ! file.hasFileExtension ("wav"))
                file = file.withFileExtension ("wav");
        
        // Show progress dialog
        double durationMinutes = durationSeconds / 60.0;
        juce::String durationText = durationMinutes >= 1.0 
            ? juce::String (durationMinutes, 1) + " minutes"
            : juce::String (durationSeconds, 0) + " seconds";
        juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::InfoIcon,
                                                "Exporting Audio",
                                                "Exporting " + durationText + "...\n\n"
                                                "This may take a moment.");
        
        // Export audio (this will block, but that's okay for now)
        bool success = audioProcessor.exportAudio (file, presetIndex, durationSeconds, format, mp3Bitrate);
        
        if (success)
        {
            juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::InfoIcon,
                                                    "Export Complete",
                                                    "Audio exported successfully to:\n" + file.getFullPathName());
        }
        else
        {
            juce::String errorMessage = "Failed to export audio.";
            if (format == BinauralAudioProcessor::ExportFormat::MP3)
            {
                errorMessage += "\n\nMP3 export requires LAME encoder to be installed.";
                errorMessage += "\n\nPlease install LAME:";
                #if JUCE_MAC
                errorMessage += "\n  brew install lame";
                #elif JUCE_LINUX
                errorMessage += "\n  sudo apt-get install lame";
                #elif JUCE_WINDOWS
                errorMessage += "\n  Download from: https://lame.sourceforge.io/";
                #endif
            }
            else
            {
                errorMessage += "\nPlease check file permissions.";
            }
            
            juce::AlertWindow::showMessageBoxAsync (juce::MessageBoxIconType::WarningIcon,
                                                    "Export Failed",
                                                    errorMessage);
        }
        
        fileChooser.reset();
    });
}

