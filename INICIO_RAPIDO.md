# 🚀 Inicio Rápido - Binaural Generator

## ✅ Checklist de Inicio

Sigue estos pasos para comenzar a desarrollar:

### 1. Verificar Requisitos

- [ ] CMake 3.22+ instalado (`cmake --version`)
- [ ] JUCE disponible en `../JUCE` (o ajustar ruta en CMakeLists.txt)
- [ ] Compilador C++17 configurado

### 2. Compilar el Proyecto

```bash
cd BinauralGenerator
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 3. Probar el Plugin

#### macOS
```bash
# Abrir en Logic Pro, GarageBand, o usar el standalone
open build/BinauralGenerator_artefacts/Release/Standalone/Binaural\ Generator.app
```

#### Windows
```cmd
# Ejecutar standalone
build\BinauralGenerator_artefacts\Release\Standalone\Binaural Generator.exe
```

### 4. Estructura de Archivos Creada

```
BinauralGenerator/
├── Source/
│   ├── PluginProcessor.h/cpp    ✅ Procesador principal
│   ├── PluginEditor.h/cpp       ✅ Interfaz gráfica básica
│   ├── BinauralOscillator.h    ✅ Oscilador sinusoidal
│   ├── BinauralGenerator.h     ✅ Generador binaural
│   └── Presets.h                ✅ Definiciones de presets
├── CMakeLists.txt              ✅ Configuración CMake
└── README.md                    ✅ Documentación
```

## 🎯 Estado Actual del Proyecto

### ✅ Completado (Fase 1)

- [x] Estructura base del proyecto
- [x] Clase AudioProcessor con parámetros
- [x] Sistema de generación de audio básico
- [x] Interfaz gráfica funcional
- [x] Osciladores izquierdo y derecho
- [x] Control de parámetros básicos

### 🔄 Próximos Pasos (Fase 2-3)

1. **Mejorar UI**
   - [ ] Diseño más moderno
   - [ ] Visualizaciones de frecuencia
   - [ ] Selector de presets

2. **Optimizaciones**
   - [ ] Smoothing de parámetros
   - [ ] Validación de rangos
   - [ ] Manejo de errores

3. **Testing**
   - [ ] Probar en diferentes DAWs
   - [ ] Validar efecto binaural
   - [ ] Verificar rendimiento

## 🐛 Solución de Problemas

### Error: "JUCE not found"
```bash
# Verificar que JUCE está en la ruta correcta
ls ../JUCE/CMakeLists.txt

# O ajustar la ruta en CMakeLists.txt línea 6:
# add_subdirectory(/ruta/completa/a/JUCE JUCE)
```

### Error de compilación
```bash
# Limpiar y recompilar
cd build
rm -rf *
cmake ..
cmake --build . --config Release
```

### Plugin no aparece en DAW
- Verificar que el plugin está en la carpeta correcta
- Reiniciar el DAW
- Verificar permisos del archivo

## 📝 Comandos Útiles

### Compilar solo el plugin
```bash
cmake --build build --target BinauralGenerator_VST3
```

### Compilar standalone
```bash
cmake --build build --target BinauralGenerator_Standalone
```

### Limpiar build
```bash
rm -rf build/*
```

## 🎓 Recursos de Aprendizaje

1. **JUCE Tutorials**: https://juce.com/learn/tutorials
2. **CMake Documentation**: https://cmake.org/documentation/
3. **Plan de Desarrollo**: `../PLAN_DESARROLLO_BINAURAL.md`

## 💡 Tips

- Usa auriculares para probar el efecto binaural completo
- Empieza con frecuencias bajas (200-500 Hz) y offsets pequeños (5-15 Hz)
- El modo Binaural es más fácil de usar que el Manual
- Guarda tus configuraciones favoritas como presets

---

**¡Listo para comenzar!** 🎉

