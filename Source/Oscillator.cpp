/*
  ==============================================================================

    Oscillator.cpp
    Created: 28 Dec 2024 6:12:13am
    Author:  gugli

  ==============================================================================
*/
#include <cmath>
#include "Oscillator.h"

namespace arrakis {

    void Oscillator::getNextAudioBlock(float* pWriteBuffer, int offset, int length)
    {
        auto deltaPerSample = m_deltaPerSample.load();
        auto endPosition = m_position + length;
        while (m_position < endPosition) {
            pWriteBuffer[offset] += (float)sin(deltaPerSample * (double)m_position) * 0.125;
            m_position++;
            offset++;
        }
    }

    void Oscillator::setFrequency(double frequency)
    {
        m_signalFrequency = frequency;
        m_deltaPerSample.store(computeDelta());
    }
}
