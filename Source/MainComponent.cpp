#include "JuceHeader.h"
#include "MainComponent.h"
#include "SineWaveComponent.h"
#include "SineWave2Component.h"
#include "AudioRecorderComponent.h"
#include "VolumeControlComponent.h"

using namespace arrakis;

//==============================================================================
MainComponent::MainComponent()
{
    m_componentFactoryMap[2] = []() { return std::make_shared<SineWaveComponent>(); };
    m_componentFactoryMap[3] = []() { return std::make_shared<SineWave2Component>(); };
    m_componentFactoryMap[4] = []() { return std::make_shared<AudioRecorderComponent>(); };
    m_componentFactoryMap[5] = []() { return std::make_shared<VolumeControlComponent>(); };

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1920, 1080);

    m_componentSelector.addItem("No component", 1);
    m_componentSelector.addItem("First Sine Wave", 2);
    m_componentSelector.addItem("Second Sine Wave", 3);
    m_componentSelector.addItem("AudioRecorder", 4);
    m_componentSelector.addItem("Volume Control", 5);
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

    listAudioDevices();

    //m_audioSelector = std::make_unique<juce::AudioDeviceSelectorComponent>(m_audioDeviceManager, 0, 2, 0, 2, true, true, true, false);
    //addAndMakeVisible(*m_audioSelector);

    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
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
    if (m_audioSelector != nullptr)
        m_audioSelector->setBounds(getLocalBounds());

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
    bufferToFill.clearActiveBufferRegion();

    auto componentPtr = m_componentPtr.load();
    if (componentPtr != nullptr) {
        componentPtr->getNextAudioBlock(bufferToFill);
    }
}

void MainComponent::listAudioDevices()
{
    juce::AudioDeviceManager audioDeviceManager;

    // Find available device types
    juce::OwnedArray<juce::AudioIODeviceType> deviceTypes;
    audioDeviceManager.createAudioDeviceTypes(deviceTypes);

    for (auto* deviceType : deviceTypes)
    {
        deviceType->scanForDevices();

        // List available output devices
        juce::StringArray outputDevices = deviceType->getDeviceNames(false); // false = output devices
        DBG("Available output devices:");
        for (const auto& device : outputDevices)
            DBG("  " << device);

        // List available input devices
        juce::StringArray inputDevices = deviceType->getDeviceNames(true); // true = input devices
        DBG("Available input devices:");
        for (const auto& device : inputDevices)
            DBG("  " << device);
    }
}

