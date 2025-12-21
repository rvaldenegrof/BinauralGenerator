# Binaural Generator - Plugin de Audio JUCE

Generador de frecuencias binaurales para plugins VST3/AU.

## 🚀 Inicio Rápido

### Requisitos Previos

- **CMake** 3.22 o superior
- **JUCE** 8.0+ (debe estar en `../JUCE`)
- **Compilador C++17**:
  - macOS: Xcode 12.4+
  - Windows: Visual Studio 2019+
  - Linux: g++ 7.0+ o Clang 6.0+

### Compilación

#### macOS

```bash
cd BinauralGenerator
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

El plugin se generará en:
- **VST3**: `build/BinauralGenerator_artefacts/Release/VST3/Binaural Generator.vst3`
- **AU**: `build/BinauralGenerator_artefacts/Release/AU/Binaural Generator.component`
- **Standalone**: `build/BinauralGenerator_artefacts/Release/Standalone/Binaural Generator.app`

#### Windows

```cmd
cd BinauralGenerator
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### Linux

```bash
cd BinauralGenerator
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Instalación

#### macOS
- **VST3**: Copiar `.vst3` a `~/Library/Audio/Plug-Ins/VST3/`
- **AU**: Copiar `.component` a `~/Library/Audio/Plug-Ins/Components/`

#### Windows
- **VST3**: Copiar `.vst3` a `C:\Program Files\Common Files\VST3\`

#### Linux
- **VST3**: Copiar `.vst3` a `~/.vst3/` o `/usr/local/lib/vst3/`

## 📁 Estructura del Proyecto

```
BinauralGenerator/
├── Source/
│   ├── PluginProcessor.h/cpp    # Procesador principal del plugin
│   ├── PluginEditor.h/cpp       # Interfaz gráfica
│   ├── BinauralOscillator.h/cpp  # Oscilador sinusoidal
│   ├── BinauralGenerator.h/cpp  # Generador binaural principal
│   └── Presets.h                # Definiciones de presets
├── CMakeLists.txt               # Configuración CMake
└── README.md                    # Este archivo
```

## 🎛️ Parámetros del Plugin

- **Base Frequency**: Frecuencia base (20-20000 Hz)
- **Binaural Offset**: Diferencia entre canales (0-100 Hz)
- **Left Volume**: Volumen canal izquierdo (-60 a 0 dB)
- **Right Volume**: Volumen canal derecho (-60 a 0 dB)
- **Master Volume**: Volumen maestro (-60 a 0 dB)
- **Mode**: Modo Binaural (automático) o Manual

## 🎧 Uso

1. Abre el plugin en tu DAW favorito
2. Ajusta la frecuencia base (recomendado: 200-500 Hz)
3. Ajusta el offset binaural según el efecto deseado:
   - **Delta** (2 Hz): Sueño profundo
   - **Theta** (6 Hz): Meditación
   - **Alpha** (10 Hz): Relajación
   - **Beta** (20 Hz): Concentración
   - **Gamma** (40 Hz): Hiperactividad
4. Usa auriculares para percibir el efecto binaural completo

## 🔧 Desarrollo

### Próximos Pasos

1. **Mejorar UI**: Diseño más moderno y visualizaciones
2. **Presets**: Implementar selector de presets en la UI
3. **Tipos de onda**: Agregar square, saw, triangle
4. **Visualizador**: Mostrar forma de onda en tiempo real
5. **Modulación**: LFO para modulación de frecuencia

### Debugging

Para debugging en macOS:
```bash
# Ejecutar standalone con debugger
lldb build/BinauralGenerator_artefacts/Debug/Standalone/Binaural\ Generator.app/Contents/MacOS/Binaural\ Generator
```

## 📚 Recursos

- [Plan de Desarrollo](./../PLAN_DESARROLLO_BINAURAL.md)
- [Resumen Ejecutivo](./../RESUMEN_EJECUTIVO_BINAURAL.md)
- [JUCE Documentation](https://juce.com/learn/documentation)
- [JUCE Tutorials](https://juce.com/learn/tutorials)

## 📝 Notas

- El plugin genera audio estéreo (no requiere entrada)
- El efecto binaural funciona mejor con auriculares
- Asegúrate de usar volúmenes seguros al probar

## 🐛 Problemas Conocidos

- Ninguno hasta el momento

## 📄 Licencia

[Especificar licencia aquí]

---

**Versión**: 1.0.0  
**Estado**: Desarrollo Activo

