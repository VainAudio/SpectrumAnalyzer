#pragma once

//-----------------------------------------------------------------------------

#include <vsa/vsa.h>
#include <juce_dsp/juce_dsp.h>
#include "AudioBufferFifo.h"
#include "SpectrumAnalyzerBuffer.h"
#include "SpectrumAnalyzerCurve.h"

//-----------------------------------------------------------------------------

BEGIN_VSA_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class SpectrumAnalyzer
 * @brief runs an fft in a time slice thread over whatever data it gets
 */
class SpectrumAnalyzer : public juce::TimeSliceClient
{
    class SpectrumAnalyzerTimeSliceThread;

public:
    explicit SpectrumAnalyzer(AudioBufferFifo<float> &source, juce::TimeSliceThread &thread, int fftSize = 11);
    explicit SpectrumAnalyzer(AudioBufferFifo<double> &source, juce::TimeSliceThread &thread, int fftSize = 11);
    ~SpectrumAnalyzer() override = default;

    SpectrumAnalyzerCurve::LockedCurve getAnalyzerCurve();

    // juce::TimeSliceClient
    int useTimeSlice() override;

private:
    bool populateConduitBuffer();
    void init();

    AudioBufferFifo<float> *m_audioFifoFloat{ nullptr };
    std::vector<float> m_conduitBuffer;

    AudioBufferFifo<double> *m_audioFifoDouble{ nullptr };
    std::vector<double> m_conduitBufferDouble;

    std::array<SpectrumAnalyzerBuffer, 5> m_buffers;

    juce::dsp::FFT m_fft;
    juce::dsp::WindowingFunction<float> m_windowingFunction{ static_cast<std::size_t>(m_fft.getSize()),
                                                             juce::dsp::WindowingFunction<float>::hann, true };

    SpectrumAnalyzerCurve m_analyzerCurve{ m_fft.getSize() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzer)
};

//-----------------------------------------------------------------------------

END_VSA_NAMESPACE

//-----------------------------------------------------------------------------
