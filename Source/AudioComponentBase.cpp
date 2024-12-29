/*
  ==============================================================================

    AudioComponentBase.cpp
    Created: 28 Dec 2024 12:58:58pm
    Author:  gugli

  ==============================================================================
*/

#include "JuceHeader.h"
#include "AudioComponentBase.h"

namespace arrakis {

    //==============================================================================
    AudioComponentBase::AudioComponentBase()
    {
    }

    void AudioComponentBase::paint(juce::Graphics& g)
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour(juce::Colours::grey);
        g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions(14.0f));
        g.drawText("AudioComponentBase", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }

    void AudioComponentBase::resized()
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

}
