/*
  ==============================================================================

    SineWave2Component.cpp
    Created: 28 Dec 2024 1:02:16pm
    Author:  gugli

  ==============================================================================
*/

#include "SineWave2Component.h"

namespace arrakis
{
    void SineWave2Component::resized()
    {
        m_slider.setBounds(50, 50, getWidth() - 100, 50);
    }

    SineWave2Component::SineWave2Component()
    {
        m_slider.setSliderStyle(juce::Slider::LinearHorizontal); // Horizontal style
        m_slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); // Textbox below the slider
        m_slider.setRange(20, 16000, 1.0); // Set the range (min, max, step size)
        m_slider.setSkewFactorFromMidPoint(1000.0);
        m_slider.setValue(m_initialFrequency); // Default value
        m_slider.setColour(juce::Slider::thumbColourId, juce::Colours::blue); // Customize thumb color

        // Add a listener to respond to slider changes
        m_slider.onValueChange = [this]()
        {
            auto frequency = m_slider.getValue();
            juce::Logger::outputDebugString("Slider Value: " + juce::String(frequency));
            m_oscillatorPtr->setFrequency(frequency);
        };        
        addAndMakeVisible(m_slider);
    }

    SineWave2Component::~SineWave2Component()
    {
    }

    void SineWave2Component::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
    {
        m_oscillatorPtr = std::make_unique<Oscillator>(sampleRate, m_initialFrequency);
    }

    void SineWave2Component::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
    {
        bufferToFill.clearActiveBufferRegion();

        auto* buffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

        m_oscillatorPtr->getNextAudioBlock(buffer, bufferToFill.startSample, bufferToFill.numSamples);
    }

    void SineWave2Component::paint(juce::Graphics& g)
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }
}

