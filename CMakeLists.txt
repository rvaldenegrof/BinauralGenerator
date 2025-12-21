cmake_minimum_required(VERSION 3.22)

project(BinauralGenerator VERSION 1.0.0)

# Add JUCE subdirectory (assuming JUCE is in ../JUCE)
add_subdirectory(../JUCE JUCE)

# Configure plugin
juce_add_plugin(BinauralGenerator
    VERSION 1.0.0
    COMPANY_NAME "BinauralAudio"
    COMPANY_EMAIL "info@binauralaudio.com"
    COMPANY_WEBSITE "https://binauralaudio.com"
    COMPANY_COPYRIGHT "Copyright (c) 2024"
    PLUGIN_MANUFACTURER_CODE Bina
    PLUGIN_CODE Bina
    PLUGIN_NAME "Binaural Generator"
    PLUGIN_DESCRIPTION "Generador de frecuencias binaurales"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Binaural Generator")

# Add source files
target_sources(BinauralGenerator
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/BinauralOscillator.cpp
        Source/BinauralGenerator.cpp)

# Compile definitions
target_compile_definitions(BinauralGenerator
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_USE_LAME_AUDIO_FORMAT=1)

# Link libraries
target_link_libraries(BinauralGenerator
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

