# Guía de construcción para Android

## Requisitos previos

- Android Studio con SDK y NDK instalados
- Java JDK compatible (Java 11 o superior)
- CMake 3.22 o superior
- JUCE 8+ (
  - puede estar en `../JUCE`
  - o en otra ruta local y ajustar `CMakeLists.txt`
)
- Un dispositivo Android o emulador con soporte de audio

## Estructura recomendada

```
BinauralGenerator/
├── Migrations/
│   ├── CMakeLists.txt
│   ├── ARCHITECTURE.md
│   ├── BUILD_GUIDE.md
│   ├── README.md
│   └── Source/
│       ├── MainComponent.h
│       ├── MainComponent.cpp
│       ├── AndroidAudioEngine.h
│       ├── AndroidAudioEngine.cpp
│       ├── AndroidTimerController.h
│       ├── AndroidTimerController.cpp
│       ├── BinauralGeneratorWrapper.h
│       └── BinauralGeneratorWrapper.cpp
└── Source/  # código original existente
```

## Build con CMake

En la carpeta `Migrations`:

```bash
cd /Users/macbook/Documents/DEV/BinauralGenerator/Migrations
mkdir -p build
cd build
cmake .. \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-31 \
  -DCMAKE_ANDROID_NDK="$ANDROID_NDK_ROOT" \
  -DCMAKE_ANDROID_STL_TYPE=c++_shared \
  -DJUCE_PATH=../JUCE
cmake --build .
```

> Si tu JUCE no está en `../JUCE`, ajusta la ruta en `CMakeLists.txt` o en `-DJUCE_PATH`.

## Notas de configuración Android

- `CMAKE_ANDROID_NDK` debe apuntar al NDK instalado
- `ANDROID_ABI` puede ser `arm64-v8a`, `armeabi-v7a`, `x86_64`
- `ANDROID_PLATFORM` debe ser compatible con tu dispositivo y la versión de JUCE

## Pruebas

- Usa `adb` o el emulador de Android Studio
- Si generas APK con CMake, instala con:

```bash
adb install -r path/to/app.apk
```

- Alternativamente, si el proyecto genera un `.aar`/`.apk`, usa Android Studio para cargarlo directamente.

## Consideraciones específicas para audio en background

- Android puede detener aplicaciones inactivas si no hay un servicio de foreground
- Agrega un servicio de reproducción si necesitas que continúe con pantalla bloqueada
- Usa notificaciones persistentes para audio continuo

## Ajustes en el manifiesto

Asegúrate de incluir permisos si la app accede a almacenamiento o audio:

- `android.permission.FOREGROUND_SERVICE`
- `android.permission.WAKE_LOCK`

Y en el servicio de primer plano:

```xml
<service android:name=".AudioForegroundService"
         android:foregroundServiceType="mediaPlayback" />
```
