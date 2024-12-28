#pragma once
#include <memory>
#include <functional>
#include <JuceHeader.h>
#include "Oscillator.h"

using namespace arrakis;

using ComponentFactory = std::function<std::unique_ptr<juce::Component>()>;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:    // implementation
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

private:
    juce::ComboBox m_componentSelector;
    std::unique_ptr<Component> m_componentPtr;
    std::unordered_map<int, ComponentFactory> m_componentFactoryMap;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
