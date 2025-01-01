#include "JuceHeader.h"
#include "AudioRecorderComponent.h"

namespace arrakis {

	void AudioRecorderComponent::resized()
	{
		auto area = getLocalBounds();
		auto childArea = area.removeFromTop(80).reduced(10); // Top 50px, with 10px padding
		m_startButton.setBounds(childArea);
	}

	void AudioRecorderComponent::saveAudioToWaveFile(const juce::AudioBuffer<float>& audioBuffer, const juce::File& outputFile, double sampleRate)
	{
		// Ensure the output file's parent directory exists
		if (!outputFile.getParentDirectory().createDirectory())
		{
			DBG("Failed to create parent directory for the output file.");
			return;
		}

		// Create an output stream for the file
		std::unique_ptr<juce::FileOutputStream> fileStream(outputFile.createOutputStream());
		if (fileStream == nullptr)
		{
			DBG("Failed to create output stream for the file.");
			return;
		}

		// Create a WAV audio format writer
		juce::WavAudioFormat wavFormat;
		std::unique_ptr<juce::AudioFormatWriter> writer(wavFormat.createWriterFor(
			fileStream.get(),          // The output stream
			sampleRate,                // Sample rate
			audioBuffer.getNumChannels(), // Number of channels
			16,                        // Bits per sample
			{},                        // Metadata (empty for now)
			0                          // Compression level (0 = lossless)
		));

		if (writer == nullptr)
		{
			DBG("Failed to create WAV writer.");
			return;
		}

		// Write the audio buffer to the file
		fileStream.release(); // Release ownership of the stream, so the writer can use it
		writer->writeFromAudioSampleBuffer(audioBuffer, 0, audioBuffer.getNumSamples());

		DBG("Audio saved successfully to " << outputFile.getFullPathName());
	}

	void AudioRecorderComponent::createTestWaveFile()
	{
		// Example: Create a sine wave buffer
		const double sampleRate = 44100.0;
		const int numSamples = 44100; // 1 second
		const int numChannels = 1;

		juce::AudioBuffer<float> audioBuffer(numChannels, numSamples);
		audioBuffer.clear();

		// Generate a sine wave
		const float frequency = 440.0f; // A4 pitch
		for (int i = 0; i < numSamples; ++i)
		{
			float sampleValue = std::sin(2.0 * juce::MathConstants<double>::pi * frequency * i / sampleRate);
			for (int channel = 0; channel < numChannels; ++channel)
			{
				audioBuffer.setSample(channel, i, sampleValue);
			}
		}

		// Output file
		juce::File outputFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getChildFile("test_wave_file.wav");

		// Save the buffer to a WAV file
		saveAudioToWaveFile(audioBuffer, outputFile, sampleRate);
	}

	AudioRecorderComponent::AudioRecorderComponent()
		: m_startButton("Start")
	{
		addAndMakeVisible(m_startButton);

		m_startButton.onClick = [this]() {
			juce::Logger::writeToLog("Start recording.");

			const auto samplesPerTick = 480;
			auto totNumSamples = samplesPerTick * 100 * 10;
			m_oscillatorPtr = std::make_unique<Oscillator>(48000, 440);
			m_audioBufferPtr = std::make_unique<juce::AudioBuffer<float>>(1, totNumSamples);
			m_audioBufferPtr->clear();
			int offset = 0;

			float sampleBuffer[samplesPerTick];

			for (int i = 0; i < 10; i++) {	// 10 seconds
				for (int j = 0; j < 100; j++) {	// 100 ticks per seconds
					for (int k = 0; k < samplesPerTick; k++) {
						sampleBuffer[k] = 0;
					}
					m_oscillatorPtr->getNextAudioBlock(sampleBuffer, sampleBuffer, 0, samplesPerTick);
					m_audioBufferPtr->copyFrom(0, offset, sampleBuffer, samplesPerTick);
					offset += samplesPerTick;
					//for (int k = 0; k < samplesPerTick; k++) {
					//	m_audioBufferPtr->setSample(0, offset, sampleBuffer[k]);
					//	offset++;
					//}
				}
			}

			juce::File outputFile = juce::File::getSpecialLocation(juce::File::userDesktopDirectory).getChildFile("test_wave_file.wav");
			saveAudioToWaveFile(*m_audioBufferPtr, outputFile, 48000);
		};
	}

	void AudioRecorderComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
	{
	}
}