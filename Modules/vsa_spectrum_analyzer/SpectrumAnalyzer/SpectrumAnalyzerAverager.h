#pragma once

#include <juce_dsp/juce_dsp.h>
#include <span>
#include <vsa/vsa.h>

//-----------------------------------------------------------------------------

BEGIN_VSA_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class SpectrumAnalyzerAverager
 * @brief calculate the average over the last n buffers. This is useful for
 * smoothing out abrupt changes in the fft output.
 *
 * getReadSpan() runs the average
 */
class SpectrumAnalyzerAverager
{
public:
    explicit SpectrumAnalyzerAverager(int fftSize, int averagerBufferCount = 5);

    void pushRawFftCurve(std::span<const float> fftCurve);

    std::span<const float> getReadSpan();

private:
    juce::AudioBuffer<float> m_outputBuffer;
    juce::AudioBuffer<float> m_averagerBuffer;
    int m_averagerChannelIndex{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzerAverager)
};

//-----------------------------------------------------------------------------

END_VSA_NAMESPACE

//-----------------------------------------------------------------------------
