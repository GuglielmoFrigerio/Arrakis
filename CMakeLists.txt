cmake_minimum_required(VERSION 3.24)

# Project name and version
project(Arrakis VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# We're going to use CPM as our package manager to bring in JUCE
# Check to see if we have CPM installed already.  Bring it in if we don't.
set(CPM_DOWNLOAD_VERSION 0.34.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif ()

include(${CPM_DOWNLOAD_LOCATION})

# Bring in JUCE locally
CPMAddPackage(
    NAME juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG origin/master
)

# Add source files
set(SourceFiles 
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MainComponent.h
    Source/AudioComponentBase.h
    Source/AudioComponentBase.cpp
    Source/Oscillator.h
    Source/Oscillator.cpp
    Source/SineWaveComponent.cpp
    Source/SineWaveComponent.h
    Source/SineWave2Component.cpp
    Source/SineWave2Component.h
    Source/JuceHeader.h
 "Source/AudioRecorderComponent.h" "Source/AudioRecorderComponent.cpp")

# Define the target
juce_add_gui_app(Arrakis
    PRODUCT_NAME "Arrakis" # Replace with your product name
)

# How we want our SourceFiles to appear in our IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SourceFiles})

# Make the SourceFiles buildable
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

# These are some toggleable options from the JUCE CMake API
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link JUCE modules
target_link_libraries(Arrakis PRIVATE
    juce::juce_animation
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_osc
    juce::juce_video
)

target_compile_definitions(Arrakis PRIVATE
    JUCE_APPLICATION_NAME_STRING="MyJuceApp"
    JUCE_APPLICATION_VERSION_STRING="1.0"
)

#target_compile_options(Arrakis PRIVATE -Wall)

if(WIN32)
    target_compile_definitions(Arrakis PRIVATE JUCE_WINDOWS=1)
elseif(APPLE)
    target_compile_definitions(Arrakis PRIVATE JUCE_MAC=1)
elseif(UNIX)
    target_compile_definitions(Arrakis PRIVATE JUCE_LINUX=1)
endif()



