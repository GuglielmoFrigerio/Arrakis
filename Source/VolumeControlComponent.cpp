#include "JuceHeader.h"
#include "VolumeControlComponent.h"

namespace arrakis {

	void VolumeControlComponent::setupSlider(juce::Slider& slider, double minimum, double maximum, double interval)
	{
		slider.setSliderStyle(juce::Slider::LinearHorizontal); // Horizontal style
		slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); // Textbox below the slider
		slider.setRange(minimum, maximum, interval); // Set the range (min, max, step size)
		slider.setValue(1.0); // Default value
		slider.setColour(juce::Slider::thumbColourId, juce::Colours::blue); // Customize thumb color
	}

	void VolumeControlComponent::setupDecibelSlider(DecibelSlider& decibelSlider)
	{
		decibelSlider.setRange(-80, 20);
		decibelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 100, 20);
		decibelSlider.onValueChange = [this] {
			auto gain = juce::Decibels::decibelsToGain((float)m_decibelSlider.getValue());
			m_upperOscPtr->setVolume(gain);
			juce::Logger::getCurrentLogger()->writeToLog(juce::String::formatted("gain is: %.8f", gain));
		};
		decibelSlider.setValue(juce::Decibels::gainToDecibels(1));
	}

	void VolumeControlComponent::resized()
	{
		auto area = getLocalBounds();

		auto linearArea = area.removeFromBottom(50).reduced(10); // Top 50px, with 10px padding
		m_linearSlider.setBounds(linearArea);

		auto logArea = area.removeFromBottom(50).reduced(10);
		m_decibelSlider.setBounds(logArea);

		auto panArea = area.removeFromBottom(50).reduced(10);
		m_panSlider.setBounds(panArea);
	}

	void VolumeControlComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
	{
		auto* pLeftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
		auto* pRightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

		m_lowerOscPtr->getNextAudioBlock(pLeftBuffer, pRightBuffer, bufferToFill.startSample, bufferToFill.numSamples);
		m_upperOscPtr->getNextAudioBlock(pLeftBuffer, pRightBuffer, bufferToFill.startSample, bufferToFill.numSamples);
	}

	void VolumeControlComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		m_lowerOscPtr = std::make_unique<Oscillator>(sampleRate, 440.0);
		m_upperOscPtr = std::make_unique<Oscillator>(sampleRate, 880.0);
	}

	VolumeControlComponent::VolumeControlComponent()
	{
		setupSlider(m_linearSlider, 0.0, 1.0, 0.001);
		setupSlider(m_panSlider, -100, 100, 1);
		setupDecibelSlider(m_decibelSlider);
		addAndMakeVisible(m_linearSlider);
		addAndMakeVisible(m_decibelSlider);
		addAndMakeVisible(m_panSlider);

		m_linearSlider.onValueChange = [this]() {
			auto value = m_linearSlider.getValue();
			m_lowerOscPtr->setVolume(value);
		};

		m_panSlider.onValueChange = [this]() {
			auto value = m_panSlider.getValue();
			m_upperOscPtr->setPan((int)value);
		};
	}
}