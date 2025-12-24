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

    // Solfeggio Frequencies
    const Preset SOLFEGGIO_174 = {
        "174 Hz",
        174.0f,
        1.5f,
        "Alivio del dolor, conexión a tierra, seguridad"
    };

    const Preset SOLFEGGIO_285 = {
        "285 Hz",
        285.0f,
        1.5f,
        "Curación celular, regeneración de tejidos, vitalidad"
    };

    const Preset SOLFEGGIO_396 = {
        "396 Hz",
        396.0f,
        1.5f,
        "Liberación de miedo y culpa, pensamientos negativos"
    };

    const Preset SOLFEGGIO_417 = {
        "417 Hz",
        417.0f,
        1.5f,
        "Facilitar cambios, deshacer situaciones, nuevos comienzos"
    };

    const Preset SOLFEGGIO_528 = {
        "528 Hz",
        528.0f,
        1.5f,
        "Reparación del ADN, armonía, frecuencia del amor"
    };

    const Preset SOLFEGGIO_639 = {
        "639 Hz",
        639.0f,
        1.5f,
        "Conectar relaciones, armonía interpersonal, comunidad"
    };

    const Preset SOLFEGGIO_741 = {
        "741 Hz",
        741.0f,
        1.5f,
        "Despertar creatividad, autoexpresión, limpieza celular"
    };

    const Preset SOLFEGGIO_852 = {
        "852 Hz",
        852.0f,
        1.5f,
        "Despertar intuición (tercer ojo), sabiduría, pensamientos positivos"
    };

    const Preset SOLFEGGIO_963 = {
        "963 Hz",
        963.0f,
        1.5f,
        "Conexión superior, unidad, despertar a un estado perfecto"
    };

    // Schumann Resonance
    const Preset SCHUMANN_RESONANCE = {
        "Schumann Resonance",
        7.83f,
        0.5f,
        "Resonancia Schumann: Fundamental 7.83 Hz. Armónicos: 14.07, 20.25, 26.41, 32.45 Hz"
    };

    // Array of all presets
    const Preset ALL_PRESETS[] = {
        DELTA,
        THETA,
        ALPHA,
        BETA,
        GAMMA,
        SOLFEGGIO_174,
        SOLFEGGIO_285,
        SOLFEGGIO_396,
        SOLFEGGIO_417,
        SOLFEGGIO_528,
        SOLFEGGIO_639,
        SOLFEGGIO_741,
        SOLFEGGIO_852,
        SOLFEGGIO_963,
        SCHUMANN_RESONANCE
    };

    const int NUM_PRESETS = 15;
}

