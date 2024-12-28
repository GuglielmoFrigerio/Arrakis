#include "MainComponent.h"
#include "SineWaveComponent.h"
#include "SineWave2Component.h"

using namespace arrakis;

//==============================================================================
MainComponent::MainComponent()
{
    m_componentFactoryMap[2] = []() { return std::make_unique<SineWaveComponent>(); };
    m_componentFactoryMap[3] = []() { return std::make_unique<SineWave2Component>(); };

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1920, 1080);

    m_componentSelector.addItem("No component", 1);
    m_componentSelector.addItem("First Sine Wave", 2);
    m_componentSelector.addItem("Second Sine Wave", 3);
    m_componentSelector.setSelectedId(1);

    addAndMakeVisible(m_componentSelector);

    m_componentSelector.onChange = [this]()
    {
        auto selectedId = m_componentSelector.getSelectedId();
        if (selectedId == 2) {
            m_componentPtr = std::make_unique<SineWaveComponent>();
            addAndMakeVisible(*m_componentPtr);
        }
        else if (selectedId == 3) {
            m_componentPtr = std::make_unique<SineWave2Component>();
            addAndMakeVisible(*m_componentPtr);
        }
        resized();
    };

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

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto childArea = area.removeFromTop(80).reduced(10); // Top 50px, with 10px padding
    // Set the bounds for the child component
    m_componentSelector.setBounds(childArea);

    if (m_componentPtr != nullptr) {
        m_componentPtr->setBounds(area);
    }
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void MainComponent::releaseResources()
{
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
}

