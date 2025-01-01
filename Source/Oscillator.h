/*
  ==============================================================================

    Oscillator.h
    Created: 28 Dec 2024 6:12:13am
    Author:  gugli

  ==============================================================================
*/

#pragma once

namespace arrakis {
    class Oscillator {
    private:
        double m_sampleFrequency;
        double m_signalFrequency;
        double m_position = 0.0;
        std::atomic<double> m_deltaPerSample;
        std::atomic<double> m_volume;
        int m_counter = 0;
        double m_doublePi;
        double m_leftGain = 1.0;
        double m_rightGain = 1.0;
        int m_pan; // -100 to +100

    private:
        double computeDelta() {
            return (m_doublePi * m_signalFrequency) / m_sampleFrequency;
        }

    public:
        Oscillator(double sampleFrequency, double signalFrequency) :
            m_sampleFrequency(sampleFrequency),
            m_signalFrequency(signalFrequency),
            m_doublePi(std::numbers::pi * 2.0),
            m_volume(1.0)
        {

            m_deltaPerSample.store(computeDelta());
        }

        void getNextAudioBlock(float* pLeftWriteBuffer, float* pWriteWriteBuffer, int offset, int length);

        void setFrequency(double frequency);

        double getFrequency() const {
            return m_signalFrequency;
        }

        void setVolume(double volume) {
            m_volume.store(volume);
        }

        void setPan(int pan);
    };
}