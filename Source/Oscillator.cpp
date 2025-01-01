/*
  ==============================================================================

    Oscillator.cpp
    Created: 28 Dec 2024 6:12:13am
    Author:  gugli

  ==============================================================================
*/
#include "JuceHeader.h"
#include "Oscillator.h"

namespace arrakis {

    void Oscillator::getNextAudioBlock(float* pLeftWriteBuffer, float* pWriteWriteBuffer, int offset, int length)
    {
        auto deltaPerSample = m_deltaPerSample.load();
        auto endPosition = offset + length;
        while (offset < endPosition) {
            auto sample = (float)std::sin(m_position) * m_volume.load();
            pLeftWriteBuffer[offset] += sample * m_leftGain;
            pWriteWriteBuffer[offset] += sample * m_rightGain;
            m_position = std::remainder(m_position + deltaPerSample, m_doublePi);
            offset++;
        }
    }

    void Oscillator::setFrequency(double frequency)
    {
        m_signalFrequency = frequency;
        m_deltaPerSample.store(computeDelta());
    }

    void Oscillator::setPan(int pan)
    {
        if (pan >= 0) {
            m_rightGain = 1.0;
            m_leftGain = juce::Decibels::decibelsToGain<double>((double)-pan);
        }
        else {
            m_leftGain = 1.0;
            m_rightGain = juce::Decibels::decibelsToGain<double>((double)pan);
        }
    }
}
