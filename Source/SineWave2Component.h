/*
  ==============================================================================

    SineWave2Component.h
    Created: 28 Dec 2024 1:02:16pm
    Author:  gugli

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioComponentBase.h"
#include "Oscillator.h"

namespace arrakis 
{
    class SineWave2Component : public AudioComponentBase 
    {
    private:    // fields
        juce::Slider m_slider;
        std::unique_ptr<Oscillator> m_oscillatorPtr;
        double m_initialFrequency = 440.0;

    private:    // implementation
        void resized() override;

    public:
        SineWave2Component();
        ~SineWave2Component() override;


        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

        void releaseResources() override;

        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

        void paint(juce::Graphics& g) override;
    };

}
