#include "MainComponent.h"
#include "SineWaveComponent.h"
#include "SineWave2Component.h"

using namespace arrakis;

//==============================================================================
MainComponent::MainComponent()
{
    m_componentFactoryMap[2] = []() { return std::make_shared<SineWaveComponent>(); };
    m_componentFactoryMap[3] = []() { return std::make_shared<SineWave2Component>(); };

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
        auto componentPtr = m_componentPtr.load();
        if (componentPtr != nullptr)
            m_componentPtr = nullptr;

        auto it = m_componentFactoryMap.find(selectedId);
        if (it != m_componentFactoryMap.end()) {
            auto componentPtr = it->second();
            componentPtr->prepareToPlay(m_samplesPerBlockExpected, m_sampleRate);
            addAndMakeVisible(*componentPtr);
            m_componentPtr.store(componentPtr);
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
    shutdownAudio();
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

    auto componentPtr = m_componentPtr.load();
    if (componentPtr != nullptr) {
        componentPtr->setBounds(area);
    }
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_samplesPerBlockExpected = samplesPerBlockExpected;
    m_sampleRate = sampleRate;
}

void MainComponent::releaseResources()
{
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto componentPtr = m_componentPtr.load();
    if (componentPtr != nullptr) {
        componentPtr->getNextAudioBlock(bufferToFill);
    }
}

