#pragma once

#include <juce_core/juce_core.h>

//==============================================================================
/**
    Preset definitions for common binaural frequencies
*/
namespace BinauralPresets
{
    struct Preset
    {
        juce::String name;
        float baseFrequency;
        float offset;
        juce::String description;
    };

    // Common binaural presets
    const Preset DELTA = {
        "Delta",
        200.0f,
        2.0f,
        "Deep Sleep (0.5-4 Hz)"
    };

    const Preset THETA = {
        "Theta",
        200.0f,
        6.0f,
        "Deep Meditation (4-8 Hz)"
    };

    const Preset ALPHA = {
        "Alpha",
        200.0f,
        10.0f,
        "Relaxation (8-13 Hz)"
    };

    const Preset BETA = {
        "Beta",
        200.0f,
        20.0f,
        "Concentration (13-30 Hz)"
    };

    const Preset GAMMA = {
        "Gamma",
        200.0f,
        40.0f,
        "Hyperactivity (30-100 Hz)"
    };

    // Array of all presets
    const Preset ALL_PRESETS[] = {
        DELTA,
        THETA,
        ALPHA,
        BETA,
        GAMMA
    };

    const int NUM_PRESETS = 5;
}

