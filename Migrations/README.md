# Migración a Android: Binaural Generator

Este directorio contiene el esquema y los archivos necesarios para iniciar la migración del proyecto `BinauralGenerator` a una aplicación Android.

## Qué incluye

- `CMakeLists.txt`: plantilla de proyecto Android JUCE
- `ARCHITECTURE.md`: arquitectura y esquema de clases
- `BUILD_GUIDE.md`: guía de construcción para Android
- `Source/`: esqueleto de clases necesarias para la nueva app Android

## Objetivo

Crear una app Android que:

- reproduzca audio binaural en segundo plano
- funcione con pantalla bloqueada
- tenga temporizador integrado
- no dependa de exportación de archivos

## Uso

1. Copia o adapta `CMakeLists.txt` para tu ruta de JUCE
2. Copia los archivos de código desde `Migrations/Source` o úsalos como plantilla
3. Ajusta el manifest Android/servicio si necesitas reproducción en foreground
4. Sigue los pasos de `BUILD_GUIDE.md`
