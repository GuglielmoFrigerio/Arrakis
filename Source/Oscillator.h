/*
  ==============================================================================

    Oscillator.h
    Created: 28 Dec 2024 6:12:13am
    Author:  gugli

  ==============================================================================
*/

#pragma once
#include <numbers>
#include <atomic>

namespace arrakis {
    class Oscillator {
    private:
        double m_sampleFrequency;
        double m_signalFrequency;
        double m_position = 0.0;
        std::atomic<double> m_deltaPerSample;
        int m_counter = 0;
        double m_doublePi;

    private:
        double computeDelta() {
            return (m_doublePi * m_signalFrequency) / m_sampleFrequency;
        }

    public:
        Oscillator(double sampleFrequency, double signalFrequency) :
            m_sampleFrequency(sampleFrequency),
            m_signalFrequency(signalFrequency),
            m_doublePi(std::numbers::pi * 2.0)
        {

            m_deltaPerSample.store(computeDelta());
        }

        void getNextAudioBlock(float* pWriteBuffer, int offset, int length);

        void setFrequency(double frequency);

        double getFrequency() const {
            return m_signalFrequency;
        }
    };
}