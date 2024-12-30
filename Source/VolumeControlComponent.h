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

	private:	// implementation

		void setupSlider(juce::Slider& slider);
		void setupDecibelSlider(DecibelSlider& decibelSlider);
		void resized() override;

		// Inherited via AudioComponentBase
		void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
		void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	public:
		VolumeControlComponent();
	};
}