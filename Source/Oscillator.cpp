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

    void Oscillator::getNextAudioBlock(float* pWriteBuffer, int offset, int length)
    {
        auto deltaPerSample = m_deltaPerSample.load();
        auto endPosition = offset + length;
        while (offset < endPosition) {
            pWriteBuffer[offset] += (float)std::sin(m_position) * m_volume.load();
            m_position = std::remainder(m_position + deltaPerSample, m_doublePi);
            offset++;
        }
    }

    void Oscillator::setFrequency(double frequency)
    {
        m_signalFrequency = frequency;
        m_deltaPerSample.store(computeDelta());
    }
}
