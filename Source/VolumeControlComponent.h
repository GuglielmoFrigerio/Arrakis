#pragma once
#include <memory>
#include "AudioComponentBase.h"
#include "Oscillator.h"
#include "DecibelSlider.h"

namespace arrakis {
	class VolumeControlComponent : public AudioComponentBase {
	private:	// fields
		std::unique_ptr<Oscillator> m_lowerOscPtr;
		std::unique_ptr<Oscillator> m_upperOscPtr;
		juce::Slider m_linearSlider;
		DecibelSlider m_decibelSlider;
		juce::Slider m_panSlider;

	private:	// implementation

		void setupSlider(juce::Slider& slider, double minimum, double maximum, double interval);
		void setupDecibelSlider(DecibelSlider& decibelSlider);
		void resized() override;

		// Inherited via AudioComponentBase
		void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	public:
		VolumeControlComponent();
	};
}