# Arquitectura y Flujo de la Aplicación Binaural Generator

## Tabla de Contenidos
1. [Arquitectura General](#arquitectura-general)
2. [Componentes Principales](#componentes-principales)
3. [Flujo de Procesamiento de Audio](#flujo-de-procesamiento-de-audio)
4. [Flujo de Interacción del Usuario](#flujo-de-interacción-del-usuario)
5. [Flujo de Exportación de Audio](#flujo-de-exportación-de-audio)
6. [Gestión de Parámetros](#gestión-de-parámetros)
7. [Diagramas de Flujo](#diagramas-de-flujo)

---

## Arquitectura General

### Estructura del Proyecto

```
BinauralGenerator/
├── CMakeLists.txt              # Configuración de compilación
├── Source/
│   ├── PluginProcessor.h/cpp   # Procesador de audio principal
│   ├── PluginEditor.h/cpp      # Interfaz de usuario
│   ├── BinauralGenerator.h/cpp # Generador binaural (lógica de síntesis)
│   ├── BinauralOscillator.h/cpp # Oscilador individual (onda seno)
│   └── Presets.h               # Definiciones de presets
└── build/                      # Archivos de compilación
```

### Arquitectura en Capas

```
┌─────────────────────────────────────────────────────────────┐
│                    CAPA DE PRESENTACIÓN                      │
│                  (PluginEditor - UI)                        │
│  - Sliders, Botones, ComboBox                               │
│  - Gestión de eventos del usuario                           │
│  - Visualización de progreso                                │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        │ AudioProcessorValueTreeState
                        │ (Sincronización thread-safe)
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                  CAPA DE CONTROL                            │
│              (PluginProcessor)                               │
│  - Gestión de parámetros                                    │
│  - Aplicación de presets                                    │
│  - Exportación de audio                                     │
│  - Ciclo de vida del plugin                                 │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        │ BinauralGenerator
                        │
┌───────────────────────▼─────────────────────────────────────┐
│              CAPA DE PROCESAMIENTO                          │
│            (BinauralGenerator)                               │
│  - Gestión de dos osciladores (L/R)                         │
│  - Aplicación de ganancia maestra                           │
│  - Modo Binaural vs Manual                                   │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        │ BinauralOscillator (x2)
                        │
┌───────────────────────▼─────────────────────────────────────┐
│              CAPA DE GENERACIÓN                              │
│         (BinauralOscillator)                                │
│  - Generación de onda seno                                  │
│  - Control de frecuencia y amplitud                         │
└─────────────────────────────────────────────────────────────┘
```

---

## Componentes Principales

### 1. BinauralOscillator
**Responsabilidad**: Generar una onda seno pura

**Características**:
- Utiliza `juce::dsp::Oscillator<float>` para generar la onda
- Control de frecuencia (Hz)
- Control de amplitud mediante `juce::dsp::Gain<float>`
- Preparación para diferentes sample rates

**Flujo interno**:
```
setFrequency() → Oscillator.setFrequency()
setAmplitude() → Gain.setGainLinear()
process() → Oscillator.process() → Gain.process() → Output
```

### 2. BinauralGenerator
**Responsabilidad**: Gestionar dos osciladores para crear el efecto binaural

**Componentes**:
- `leftOscillator`: Oscilador para canal izquierdo
- `rightOscillator`: Oscilador para canal derecho
- `masterGain`: Ganancia maestra aplicada a ambos canales

**Modos de Operación**:
- **Binaural Mode**: 
  - Izquierdo: `baseFrequency`
  - Derecho: `baseFrequency + binauralOffset`
  - El offset crea el "beat" binaural percibido
  
- **Manual Mode**:
  - Frecuencias independientes para cada canal
  - Control total del usuario

**Flujo de procesamiento**:
```
Input: ProcessContext (buffer de audio)
  ↓
1. Limpiar buffer de salida
  ↓
2. Procesar canal izquierdo (leftOscillator)
  ↓
3. Procesar canal derecho (rightOscillator)
  ↓
4. Aplicar ganancia maestra (masterGain)
  ↓
Output: Audio estéreo con frecuencias binaurales
```

### 3. BinauralAudioProcessor
**Responsabilidad**: Procesador principal de audio según el estándar JUCE

**Funciones clave**:
- `prepareToPlay()`: Inicialización cuando el host inicia reproducción
- `processBlock()`: Procesamiento de cada bloque de audio
- `applyPreset()`: Aplicación de presets predefinidos
- `exportAudio()`: Exportación de audio a archivo (WAV/MP3)

**Parámetros gestionados**:
- `BASE_FREQUENCY_ID`: Frecuencia base (20-20000 Hz)
- `BINAURAL_OFFSET_ID`: Offset binaural (0-100 Hz)
- `LEFT_VOLUME_ID`: Volumen canal izquierdo (-60 a 0 dB)
- `RIGHT_VOLUME_ID`: Volumen canal derecho (-60 a 0 dB)
- `MASTER_VOLUME_ID`: Volumen maestro (-60 a 0 dB)
- `MODE_ID`: Modo Binaural/Manual (bool)
- `MUTE_ID`: Silenciar audio (bool)

**AudioProcessorValueTreeState**:
- Sistema thread-safe para sincronizar parámetros entre UI y procesador
- Permite automatización en DAWs
- Gestión de estado persistente

### 4. BinauralAudioProcessorEditor
**Responsabilidad**: Interfaz gráfica de usuario

**Componentes UI**:
- **Controles de síntesis**:
  - `baseFrequencySlider`: Control de frecuencia base
  - `binauralOffsetSlider`: Control de offset binaural
  - `leftVolumeSlider`: Volumen izquierdo
  - `rightVolumeSlider`: Volumen derecho
  - `masterVolumeSlider`: Volumen maestro
  - `modeToggle`: Cambio entre modo Binaural/Manual
  - `muteButton`: Botón de silencio
  - `presetComboBox`: Selector de presets

- **Controles de exportación** (solo Standalone):
  - `durationSlider`: Duración en minutos (0-120)
  - `formatComboBox`: Formato (WAV/MP3)
  - `mp3BitrateComboBox`: Bitrate MP3 (128/192/256/320 kbps)
  - `exportButton`: Iniciar exportación
  - `progressBar`: Barra de progreso visual

**Attachments**:
- `SliderAttachment`: Conecta sliders con parámetros del procesador
- `ButtonAttachment`: Conecta botones con parámetros del procesador
- Sincronización automática bidireccional

### 5. Presets.h
**Responsabilidad**: Definir presets predefinidos de frecuencias binaurales

**Estructura de Preset**:
```cpp
struct Preset {
    juce::String name;        // Nombre del preset
    float baseFrequency;      // Frecuencia base en Hz
    float offset;             // Offset binaural en Hz
    juce::String description; // Descripción del efecto
};
```

**Presets disponibles**:
- **Binaurales tradicionales**: Delta, Theta, Alpha, Beta, Gamma
- **Frecuencias Solfeggio**: 174, 285, 396, 417, 528, 639, 741, 852, 963 Hz

---

## Flujo de Procesamiento de Audio

### Ciclo de Vida del Plugin

```
1. CONSTRUCCIÓN
   └─> BinauralAudioProcessor()
       └─> createParameterLayout()
           └─> Crea AudioProcessorValueTreeState con todos los parámetros

2. INICIALIZACIÓN (cuando el host inicia reproducción)
   └─> prepareToPlay(sampleRate, samplesPerBlock)
       └─> binauralGenerator.prepare()
           └─> leftOscillator.prepare()
           └─> rightOscillator.prepare()
           └─> masterGain.prepare()

3. PROCESAMIENTO (por cada bloque de audio)
   └─> processBlock(buffer, midiMessages)
       ├─> Verificar si está muteado → Si sí, limpiar buffer y retornar
       ├─> Leer parámetros desde ValueTreeState
       ├─> Actualizar BinauralGenerator con parámetros
       └─> binauralGenerator.process(context)
           ├─> leftOscillator.process() → Genera onda seno izquierda
           ├─> rightOscillator.process() → Genera onda seno derecha
           └─> masterGain.process() → Aplica ganancia final

4. FINALIZACIÓN
   └─> releaseResources()
       └─> binauralGenerator.reset()
```

### Flujo Detallado de processBlock()

```
┌─────────────────────────────────────────────────────────────┐
│ Host DAW llama processBlock(buffer, midiMessages)           │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        ▼
        ┌───────────────────────────────┐
        │ ¿Está muteado?                │
        └───────┬───────────────┬───────┘
                │ Sí            │ No
                ▼               ▼
        ┌──────────────┐   ┌──────────────────────────────┐
        │ buffer.clear()│   │ Leer parámetros del ValueTree│
        │ return        │   │ - baseFrequency              │
        └──────────────┘   │ - binauralOffset             │
                           │ - leftVolume                 │
                           │ - rightVolume                │
                           │ - masterVolume               │
                           │ - mode                       │
                           └───────┬──────────────────────┘
                                   │
                                   ▼
                    ┌──────────────────────────────┐
                    │ Actualizar BinauralGenerator │
                    │ - setBaseFrequency()         │
                    │ - setBinauralOffset()        │
                    │ - setLeftVolume()            │
                    │ - setRightVolume()           │
                    │ - setMasterVolume()          │
                    │ - setMode()                  │
                    └───────┬──────────────────────┘
                            │
                            ▼
            ┌───────────────────────────────────────┐
            │ binauralGenerator.process(context)     │
            │                                        │
            │ 1. Limpiar outputBlock                 │
            │ 2. Crear AudioBlock para L y R        │
            │ 3. leftOscillator.process(leftBlock)   │
            │ 4. rightOscillator.process(rightBlock) │
            │ 5. masterGain.process(context)         │
            └───────┬───────────────────────────────┘
                    │
                    ▼
        ┌───────────────────────────────┐
        │ Buffer de salida listo        │
        │ (Audio estéreo con beats)     │
        └───────────────────────────────┘
```

---

## Flujo de Interacción del Usuario

### Interacción con Sliders/Botones

```
Usuario mueve slider
        │
        ▼
┌───────────────────────────────┐
│ SliderAttachment detecta cambio│
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ ValueTreeState actualiza valor │
│ (thread-safe)                  │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ processBlock() lee nuevo valor │
│ en el siguiente bloque         │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ BinauralGenerator se actualiza │
│ y genera nuevo audio           │
└───────────────────────────────┘
```

### Selección de Preset

```
Usuario selecciona preset en ComboBox
        │
        ▼
┌───────────────────────────────┐
│ presetComboBoxChanged()        │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ audioProcessor.applyPreset()  │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Obtener preset de Presets.h   │
│ - baseFrequency               │
│ - offset                      │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Actualizar ValueTreeState     │
│ - BASE_FREQUENCY_ID           │
│ - BINAURAL_OFFSET_ID          │
│ - MODE_ID = Binaural          │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ SliderAttachments actualizan  │
│ UI automáticamente            │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ processBlock() aplica cambios  │
│ en el siguiente bloque         │
└───────────────────────────────┘
```

### Botón Mute

```
Usuario presiona botón MUTE
        │
        ▼
┌───────────────────────────────┐
│ ButtonAttachment actualiza     │
│ MUTE_ID en ValueTreeState      │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ processBlock() verifica MUTE_ID│
│ if (isMuted) {                │
│     buffer.clear();           │
│     return;                   │
│ }                             │
└───────────────────────────────┘
```

---

## Flujo de Exportación de Audio

### Proceso Completo de Exportación

```
Usuario hace clic en "Export to WAV/MP3..."
        │
        ▼
┌───────────────────────────────┐
│ exportButtonClicked()         │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ startExport()                 │
│ - Obtener preset seleccionado │
│ - Obtener duración (minutos)  │
│ - Obtener formato (WAV/MP3)   │
│ - Obtener bitrate MP3         │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ FileChooser.launchAsync()     │
│ (Diálogo de guardar archivo)  │
└───────┬───────────────────────┘
        │
        ▼ Usuario selecciona archivo
┌───────────────────────────────┐
│ Crear AlertWindow con         │
│ ProgressBar                   │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Crear ExportThread            │
│ (Thread en background)        │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ ExportThread.startThread()    │
│                                │
│ En el thread:                 │
│ ┌───────────────────────────┐ │
│ │ processor.exportAudio()   │ │
│ └───────┬───────────────────┘ │
│         │                     │
│         ▼                     │
│ ┌───────────────────────────┐ │
│ │ 1. Aplicar preset si es   │ │
│ │    necesario              │ │
│ │ 2. prepareToPlay()        │ │
│ │ 3. Crear AudioFormatWriter│ │
│ │    (WAV o MP3/LAME)       │ │
│ │ 4. Loop de renderizado:   │ │
│ │    - Generar bloque audio │ │
│ │    - Escribir a archivo   │ │
│ │    - Actualizar progreso  │ │
│ │ 5. Cerrar writer          │ │
│ └───────┬───────────────────┘ │
│         │                     │
│         ▼                     │
│ ┌───────────────────────────┐ │
│ │ Llamar progressCallback() │ │
│ │ (actualiza ProgressBar)   │ │
│ └───────┬───────────────────┘ │
│         │                     │
│         ▼                     │
│ ┌───────────────────────────┐ │
│ │ Llamar completionCallback │ │
│ │ (éxito o error)           │ │
│ └───────────────────────────┘ │
└───────────────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ MessageManager::callAsync()   │
│ (Actualizar UI desde thread)  │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Cerrar AlertWindow            │
│ Mostrar mensaje de éxito/error│
└───────────────────────────────┘
```

### Detalles de exportAudio()

```
exportAudio(file, presetIndex, durationSeconds, format, mp3Bitrate)
        │
        ▼
┌───────────────────────────────┐
│ ¿presetIndex >= 0?            │
│ Sí → applyPreset(presetIndex) │
│ No → Usar parámetros actuales │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ prepareToPlay(sampleRate)     │
│ (Preparar para renderizado)   │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Crear BinauralGenerator       │
│ temporal con parámetros       │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Crear AudioFormatWriter       │
│ - WAV: WavAudioFormat         │
│ - MP3: LAMEEncoderAudioFormat │
│   (busca ejecutable LAME)     │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Loop de renderizado:          │
│                                │
│ totalSamples =                │
│   sampleRate * durationSeconds│
│                                │
│ while (samplesRendered <      │
│        totalSamples) {         │
│   - Generar bloque de audio   │
│   - tempGenerator.process()   │
│   - writer.writeFromBuffer()  │
│   - progressCallback()        │
│ }                             │
└───────┬───────────────────────┘
        │
        ▼
┌───────────────────────────────┐
│ Cerrar writer                 │
│ Retornar éxito/error          │
└───────────────────────────────┘
```

---

## Gestión de Parámetros

### AudioProcessorValueTreeState

**Propósito**: Sincronización thread-safe entre UI y procesador de audio

**Ventajas**:
- Thread-safe: La UI y el procesador pueden acceder desde diferentes threads
- Automatización: Compatible con automatización de DAWs
- Persistencia: Guarda/carga estado del plugin
- Bidireccional: Cambios en UI o procesador se sincronizan automáticamente

**Flujo de sincronización**:
```
┌──────────────┐                    ┌──────────────┐
│ UI Thread    │                    │ Audio Thread │
│ (Editor)     │                    │ (Processor)  │
└──────┬───────┘                    └──────┬───────┘
       │                                   │
       │ Cambio en slider                  │
       │                                   │
       ▼                                   │
┌──────────────────────┐                  │
│ SliderAttachment    │                  │
│ actualiza ValueTree │                  │
└──────┬───────────────┘                  │
       │                                   │
       │ ValueTreeState                   │
       │ (Thread-safe)                    │
       │                                   │
       │                                   ▼
       │                          ┌──────────────────────┐
       │                          │ processBlock() lee    │
       │                          │ parámetro            │
       │                          └──────┬───────────────┘
       │                                   │
       │                                   │ Aplica cambio
       │                                   │
       │                                   ▼
       │                          ┌──────────────────────┐
       │                          │ Audio actualizado    │
       │                          └──────────────────────┘
       │                                   │
       └───────────────────────────────────┘
```

### Tipos de Parámetros

1. **AudioParameterFloat**: Parámetros numéricos continuos
   - Frecuencia base, offset, volúmenes
   - Rango normalizado (0.0 a 1.0) internamente
   - Conversión a valores reales mediante `NormalisableRange`

2. **AudioParameterBool**: Parámetros booleanos
   - Modo (Binaural/Manual)
   - Mute

---

## Diagramas de Flujo

### Diagrama de Arquitectura Completa

```
┌─────────────────────────────────────────────────────────────────┐
│                         HOST DAW                                 │
│                    (Ableton, Logic, etc.)                        │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             │ Audio Callbacks
                             │
┌────────────────────────────▼────────────────────────────────────┐
│              BinauralAudioProcessor                              │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ AudioProcessorValueTreeState                             │  │
│  │  - BASE_FREQUENCY_ID                                     │  │
│  │  - BINAURAL_OFFSET_ID                                    │  │
│  │  - LEFT_VOLUME_ID                                        │  │
│  │  - RIGHT_VOLUME_ID                                       │  │
│  │  - MASTER_VOLUME_ID                                      │  │
│  │  - MODE_ID                                               │  │
│  │  - MUTE_ID                                               │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ BinauralGenerator                                         │  │
│  │  ┌────────────────────┐  ┌────────────────────┐         │  │
│  │  │ BinauralOscillator │  │ BinauralOscillator │         │  │
│  │  │ (Left Channel)     │  │ (Right Channel)    │         │  │
│  │  │                    │  │                    │         │  │
│  │  │ Oscillator<float>  │  │ Oscillator<float>  │         │  │
│  │  │ Gain<float>        │  │ Gain<float>        │         │  │
│  │  └────────────────────┘  └────────────────────┘         │  │
│  │                          │                              │  │
│  │                          ▼                              │  │
│  │                  Gain<float> (Master)                  │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ Funciones:                                                │  │
│  │  - prepareToPlay()                                        │  │
│  │  - processBlock()                                         │  │
│  │  - applyPreset()                                          │  │
│  │  - exportAudio()                                          │  │
│  └──────────────────────────────────────────────────────────┘  │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             │ createEditor()
                             │
┌────────────────────────────▼────────────────────────────────────┐
│              BinauralAudioProcessorEditor                       │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ UI Components:                                            │  │
│  │  - Sliders (Frequency, Offset, Volumes)                  │  │
│  │  - Buttons (Mute, Mode Toggle)                          │  │
│  │  - ComboBox (Presets)                                    │  │
│  │  - Export Controls (Standalone only)                    │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ Attachments:                                              │  │
│  │  - SliderAttachment (x5)                                 │  │
│  │  - ButtonAttachment (x2)                                  │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ Export Thread (Background):                              │  │
│  │  - ExportThread                                          │  │
│  │  - ProgressBar                                           │  │
│  │  - AlertWindow                                           │  │
│  └──────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

### Flujo de Datos en Tiempo Real

```
┌──────────────┐
│ Host DAW     │
│ processBlock │
└──────┬───────┘
       │
       │ AudioBuffer<float>
       │ (bloques de 512-2048 samples)
       │
       ▼
┌─────────────────────────────────────┐
│ BinauralAudioProcessor              │
│ processBlock(buffer, midi)          │
│                                     │
│ 1. Leer parámetros                  │
│    ValueTreeState.getRawParameter() │
│                                     │
│ 2. Actualizar BinauralGenerator     │
│    - setBaseFrequency()             │
│    - setBinauralOffset()            │
│    - setLeftVolume()                 │
│    - setRightVolume()                │
│    - setMasterVolume()               │
│                                     │
│ 3. Procesar audio                   │
│    binauralGenerator.process()      │
└──────┬──────────────────────────────┘
       │
       │
       ▼
┌─────────────────────────────────────┐
│ BinauralGenerator.process()         │
│                                     │
│ 1. Limpiar outputBlock              │
│                                     │
│ 2. leftOscillator.process()         │
│    ┌─────────────────────────────┐  │
│    │ Oscillator genera onda seno │  │
│    │ Gain aplica amplitud        │  │
│    └─────────────────────────────┘  │
│                                     │
│ 3. rightOscillator.process()        │
│    ┌─────────────────────────────┐  │
│    │ Oscillator genera onda seno │  │
│    │ (frecuencia diferente)      │  │
│    │ Gain aplica amplitud        │  │
│    └─────────────────────────────┘  │
│                                     │
│ 4. masterGain.process()             │
│    ┌─────────────────────────────┐  │
│    │ Aplica ganancia final       │  │
│    └─────────────────────────────┘  │
└──────┬──────────────────────────────┘
       │
       │ AudioBuffer<float>
       │ (estéreo con beats binaurales)
       │
       ▼
┌──────────────┐
│ Host DAW     │
│ Output       │
└──────────────┘
```

### Flujo de Exportación Detallado

```
┌─────────────────────────────────────────────────────────────┐
│ Usuario: Clic en "Export to WAV/MP3..."                    │
└───────────────────────┬─────────────────────────────────────┘
                        │
                        ▼
        ┌───────────────────────────────┐
        │ startExport()                 │
        │ - presetIndex = -1 o índice   │
        │ - durationSeconds             │
        │ - format (WAV/MP3)            │
        │ - mp3Bitrate                  │
        └───────┬───────────────────────┘
                │
                ▼
┌───────────────────────────────────────────────┐
│ FileChooser.launchAsync()                     │
│ (Diálogo de guardar archivo)                  │
└───────┬───────────────────────────────────────┘
        │
        │ Usuario selecciona archivo
        ▼
┌───────────────────────────────────────────────┐
│ Crear AlertWindow con ProgressBar             │
│ progressWindow.enterModalState()              │
└───────┬───────────────────────────────────────┘
        │
        ▼
┌───────────────────────────────────────────────┐
│ ExportThread.startThread()                    │
│ (Thread en background)                        │
└───────┬───────────────────────────────────────┘
        │
        │ En ExportThread.run():
        ▼
┌───────────────────────────────────────────────┐
│ processor.exportAudio()                       │
│                                                │
│ 1. Si presetIndex >= 0:                       │
│    applyPreset(presetIndex)                   │
│                                                │
│ 2. prepareToPlay(sampleRate, blockSize)       │
│                                                │
│ 3. Crear BinauralGenerator temporal           │
│    con parámetros actuales                    │
│                                                │
│ 4. Crear AudioFormatWriter:                   │
│    - WAV: WavAudioFormat                      │
│    - MP3: LAMEEncoderAudioFormat              │
│      (busca /usr/local/bin/lame, etc.)        │
│                                                │
│ 5. Loop de renderizado:                       │
│    totalSamples = sampleRate * durationSeconds │
│    samplesRendered = 0                         │
│                                                │
│    while (samplesRendered < totalSamples) {   │
│      a. Generar bloque de audio               │
│         tempGenerator.process()                │
│      b. writer.writeFromBuffer()               │
│      c. samplesRendered += blockSize          │
│      d. progress = samplesRendered / total     │
│      e. progressCallback(progress)             │
│    }                                           │
│                                                │
│ 6. Cerrar writer                              │
│ 7. Retornar éxito/error                       │
└───────┬───────────────────────────────────────┘
        │
        │ progressCallback()
        ▼
┌───────────────────────────────────────────────┐
│ MessageManager::callAsync()                   │
│ (Actualizar UI desde thread)                  │
│                                                │
│ progressBar->setProgressBarValue(progress)    │
└───────┬───────────────────────────────────────┘
        │
        │ completionCallback()
        ▼
┌───────────────────────────────────────────────┐
│ MessageManager::callAsync()                   │
│                                                │
│ - Cerrar progressWindow                       │
│ - Mostrar mensaje de éxito/error              │
│ - Resetear exportThread                       │
└───────────────────────────────────────────────┘
```

---

## Resumen de Flujos Clave

### 1. Inicialización
```
Constructor → createParameterLayout() → ValueTreeState creado
Host inicia → prepareToPlay() → BinauralGenerator preparado
```

### 2. Procesamiento en Tiempo Real
```
processBlock() → Leer parámetros → Actualizar generador → Procesar audio → Salida
```

### 3. Cambio de Parámetro
```
UI cambia → Attachment actualiza ValueTree → processBlock() lee nuevo valor → Audio actualizado
```

### 4. Aplicación de Preset
```
Usuario selecciona → applyPreset() → ValueTree actualizado → UI sincronizada → Audio actualizado
```

### 5. Exportación
```
Usuario inicia → FileChooser → ExportThread → exportAudio() → Renderizado → Archivo guardado
```

---

## Consideraciones de Thread Safety

### Threads en la Aplicación

1. **Message Thread (UI Thread)**:
   - PluginEditor y todos los componentes UI
   - Actualizaciones de UI deben hacerse aquí

2. **Audio Thread**:
   - processBlock() se ejecuta aquí
   - Debe ser real-time safe (sin bloqueos, sin allocaciones)

3. **Export Thread**:
   - ExportThread para exportación de audio
   - No bloquea UI ni audio thread

### Sincronización

- **AudioProcessorValueTreeState**: Thread-safe por diseño
- **MessageManager::callAsync()**: Para actualizar UI desde otros threads
- **ExportThread**: Usa callbacks para comunicarse con UI thread

---

## Dependencias Externas

### JUCE Framework
- `juce_audio_processors`: Procesamiento de audio
- `juce_dsp`: Procesamiento de señales digitales
- `juce_gui_basics`: Componentes de UI
- `juce_audio_formats`: Formatos de audio (WAV, MP3)

### LAME Encoder (Opcional)
- Requerido para exportación MP3
- Busca ejecutable en:
  - macOS: `/usr/local/bin/lame` o `/opt/homebrew/bin/lame`
  - Linux: `/usr/bin/lame` o `/usr/local/bin/lame`
  - Windows: `lame.exe` en directorio del ejecutable

---

## Conclusión

La arquitectura del Binaural Generator sigue el patrón estándar de plugins JUCE con separación clara de responsabilidades:

- **UI Layer**: Gestión de interacción del usuario
- **Control Layer**: Gestión de parámetros y lógica de negocio
- **Processing Layer**: Generación de audio binaural
- **Generation Layer**: Osciladores individuales

El uso de `AudioProcessorValueTreeState` garantiza sincronización thread-safe entre UI y procesador, mientras que el sistema de exportación permite renderizado offline sin bloquear la UI o el procesamiento en tiempo real.

