cmake_minimum_required(VERSION 3.22)

project(BinauralGeneratorAndroid VERSION 1.0.0)

# Ajusta esta ruta si JUCE no está en ../JUCE
add_subdirectory(../JUCE JUCE)

juce_add_android_app(BinauralGeneratorAndroid
    COMPANY_NAME "BinauralAudio"
    PRODUCT_NAME "Binaural Generator"
    VERSION "1.0.0"
    BUNDLE_IDENTIFIER "com.binauralaudio.binauralgenerator"
    FORMATS APP
)

# Código fuente para la app Android
target_sources(BinauralGeneratorAndroid
    PRIVATE
        Source/MainComponent.cpp
        Source/AndroidAudioEngine.cpp
        Source/AndroidTimerController.cpp
        Source/BinauralGeneratorWrapper.cpp
)

# Ajusta las definiciones según tu configuración
target_compile_definitions(BinauralGeneratorAndroid
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_ANDROID=1
)

# Enlaza los módulos JUCE necesarios para audio y UI
target_link_libraries(BinauralGeneratorAndroid
    PRIVATE
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_graphics
        juce::juce_gui_extra
)

# Propiedades adicionales de Android
set_target_properties(BinauralGeneratorAndroid PROPERTIES
    ANDROID_ARCH "arm64-v8a"
    ANDROID_PLATFORM "android-31"
)

# Nota: Si tu versión de JUCE no admite juce_add_android_app, usa juce_add_app junto a las propiedades Android.
