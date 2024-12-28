/*
  ==============================================================================

    SineWaveComponent.cpp
    Created: 28 Dec 2024 11:04:39am
    Author:  gugli

  ==============================================================================
*/

#include "SineWaveComponent.h"

namespace arrakis {

    SineWaveComponent::SineWaveComponent()
    {

        addAndMakeVisible(m_label);
        m_label.setText("Click on the left side to lower the tone or on the left side to raise the tone", juce::dontSendNotification);

        if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
            && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
        {
            juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
        }
        else
        {
            // Specify the number of input and output channels that we want to open
            setAudioChannels(2, 2);
        }
    }

    SineWaveComponent::~SineWaveComponent()
    {
        shutdownAudio();
    }

    void SineWaveComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        juce::String message;
        message << "Preparing to play audio...\n";
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
        message << " sampleRate = " << sampleRate;
        juce::Logger::getCurrentLogger()->writeToLog(message);

        m_oscillatorPtr = std::make_unique<Oscillator>(sampleRate, 1760.0);
    }

    void SineWaveComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();

        auto* buffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

        m_oscillatorPtr->getNextAudioBlock(buffer, bufferToFill.startSample, bufferToFill.numSamples);
    }

    void SineWaveComponent::releaseResources()
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    void SineWaveComponent::paint(juce::Graphics& g)
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void SineWaveComponent::resized()
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        m_currentMiddleX = getWidth() / 2;

        auto area = getLocalBounds();
        auto childArea = area.removeFromTop(50).reduced(10); // Top 50px, with 10px padding

        // Set the bounds for the child component
        m_label.setBounds(childArea);
    }

    void SineWaveComponent::mouseDown(const juce::MouseEvent& event)
    {

        auto position = event.getMouseDownPosition();
        auto x = position.getX();

        auto factor = (x > m_currentMiddleX) ? 2.0 : 0.5;

        auto frequency = m_oscillatorPtr->getFrequency();
        m_oscillatorPtr->setFrequency(frequency * factor);
    }
}