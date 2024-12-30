#pragma once
#include "AudioComponentBase.h"
#include "Oscillator.h"

namespace arrakis {
	class AudioRecorderComponent : public AudioComponentBase {
	private:	// fields
		std::unique_ptr<juce::AudioBuffer<float>> m_audioBufferPtr;
		std::unique_ptr<Oscillator> m_oscillatorPtr;
		juce::TextButton m_startButton;

	private:	// implementation
		void resized() override;
		void saveAudioToWaveFile(const juce::AudioBuffer<float>& audioBuffer, const juce::File& outputFile, double sampleRate);
		void createTestWaveFile();

	public:		// interface
		AudioRecorderComponent();

		// Inherited via AudioComponentBase
		void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	};
}