/*
  ==============================================================================

    AudioComponentBase.h
    Created: 28 Dec 2024 12:58:58pm
    Author:  gugli

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

namespace arrakis 
{
    //==============================================================================
    /*
    */
    class AudioComponentBase : public juce::AudioAppComponent
    {
    public:
        AudioComponentBase();

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioComponentBase)
    };

}
