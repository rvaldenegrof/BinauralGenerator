# Arquitectura del proyecto Android

## Contexto

Este proyecto migrado es una aplicación Android standalone basada en JUCE. Su función principal es generar audio binaural continuo, con un temporizador que detiene la reproducción después de un tiempo definido.

Se mantiene la lógica de síntesis existente en `BinauralGenerator` y `BinauralOscillator`, pero se adapta la capa de presentación y gestión de audio a Android.

## Módulos principales

### 1. UI móvil

- `MainComponent`
  - Interfaz touch-friendly
  - Controles básicos: Play / Stop, temporizador, presets, volumen y offset
  - Presenta la cuenta regresiva en formato `MM:SS`
  - Permite iniciar/detener reproducción sin exportación de archivos

### 2. Motor de audio

- `AndroidAudioEngine`
  - Inicializa `juce::AudioDeviceManager`
  - Controla el audio output de Android
  - Garantiza que el audio siga activo en segundo plano cuando la actividad se pausa

- `BinauralGeneratorWrapper`
  - Enlaza la configuración de UI con la lógica de síntesis
  - Reutiliza `BinauralGenerator` existente
  - Expone métodos como `setBaseFrequency()`, `setBinauralOffset()`, `setVolumes()` y `render()`

### 3. Temporizador

- `AndroidTimerController`
  - Controla la cuenta regresiva
  - Reporta `onTick` cada segundo
  - Llama a `onFinish` cuando termina el tiempo
  - Se integra con la UI y con el motor de audio para detener la reproducción

### 4. Integración Android

- La aplicación debe ejecutarse en un contexto Android compatible con audio en background
- Idealmente se complementa con un servicio de foreground para evitar que el sistema detenga la reproducción cuando la pantalla se bloquea
- Se recomienda usar notificación persistente si se desea robustez de audio en background

## Diagrama de clases

```text
MainComponent
├─ AndroidAudioEngine
│   ├─ juce::AudioDeviceManager
│   └─ audio callback
├─ BinauralGeneratorWrapper
│   ├─ BinauralGenerator
│   └─ BinauralOscillator
└─ AndroidTimerController
```

## Responsabilidades claras

- `MainComponent`: UI + estado de reproducción
- `AndroidAudioEngine`: gestión del dispositivo de audio y callback
- `BinauralGeneratorWrapper`: lógica de síntesis reutilizable
- `AndroidTimerController`: temporizador y finalización de sesión

## Reutilización de código actual

Copiar o referenciar desde el proyecto original:

- `Source/BinauralGenerator.h`
- `Source/BinauralOscillator.h`

Estas clases contienen la lógica DSP que debe permanecer prácticamente igual.
