#pragma once
#include <memory>
#include <JuceHeader.h>
#include "Oscillator.h"

using namespace arrakis;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent, public juce::KeyListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:    // implementation
    // Inherited via KeyListener
    bool keyPressed(const juce::KeyPress& key, Component* originatingComponent) override;
    void mouseDown(const juce::MouseEvent& event) override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::Random m_random;
    std::unique_ptr<Oscillator> m_oscillatorPtr;
    juce::Label m_label;
    int m_currentMiddleX;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

};
