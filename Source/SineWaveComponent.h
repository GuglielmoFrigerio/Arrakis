/*
  ==============================================================================

    SineWaveComponent.h
    Created: 28 Dec 2024 11:04:39am
    Author:  gugli

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Oscillator.h"
#include "AudioComponentBase.h"

namespace arrakis {
    class SineWaveComponent : public AudioComponentBase
    {
    public:
        //==============================================================================
        SineWaveComponent();
        ~SineWaveComponent() override;

        //==============================================================================
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

        //==============================================================================
        void paint(juce::Graphics& g) override;
        void resized() override;

    private:    // implementation
        void mouseDown(const juce::MouseEvent& event) override;

    private:
        std::unique_ptr<Oscillator> m_oscillatorPtr;
        juce::Label m_label;
        int m_currentMiddleX = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineWaveComponent)

    };
}
