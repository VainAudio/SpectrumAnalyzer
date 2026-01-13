#include "SpectrumAnalyzer.h"

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzer::init()
{
    const int fftSize = m_fft.getSize() * 2;
    const int stepSize = fftSize / static_cast<int>(m_buffers.size());
    for (const auto &[index, buffer] : juce::enumerate(m_buffers))
    {
        buffer.setSize(static_cast<std::size_t>(fftSize));
        const int writeSize = stepSize * static_cast<int>(index);
        if (writeSize != 0)
            buffer.skip(writeSize);
    }
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzer::SpectrumAnalyzer(vsa::AudioBufferFifo<float> &source, juce::TimeSliceThread &thread)
    : m_audioFifoFloat(&source)
{
    thread.addTimeSliceClient(this);
    init();
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzer::SpectrumAnalyzer(vsa::AudioBufferFifo<double> &source, juce::TimeSliceThread &thread)
    : m_audioFifoDouble(&source)
{
    thread.addTimeSliceClient(this);
    init();
}

//-----------------------------------------------------------------------------

int vsa::SpectrumAnalyzer::useTimeSlice()
{
    if (populateConduitBuffer())
    {
        for (auto &b : m_buffers)
        {
            b.write(m_conduitBuffer);

            if (b.isFftSpanReady())
            {
                auto fftSpan{ b.getFftSpan() };

                m_windowingFunction.multiplyWithWindowingTable(
                    fftSpan.data(), std::min(fftSpan.size(), static_cast<std::size_t>(m_fft.getSize())));
                m_fft.performFrequencyOnlyForwardTransform(fftSpan.data());

                m_analyzerCurve.pushCurve(fftSpan);
            }
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------
bool vsa::SpectrumAnalyzer::populateConduitBuffer()
{
    if (m_audioFifoFloat && m_audioFifoFloat->getAvailableSamples() > 0)
    {
        const int size{ std::min(m_buffers.front().getSize(), m_audioFifoFloat->getAvailableSamples()) };

        m_conduitBuffer.clear();
        m_conduitBuffer.assign(static_cast<std::size_t>(size), 0.0f);

        m_audioFifoFloat->popAudioData(m_conduitBuffer);

        return true;
    }

    if (m_audioFifoDouble && m_audioFifoDouble->getAvailableSamples() > 0)
    {
        const int size{ std::min(m_buffers.front().getSize(), m_audioFifoDouble->getAvailableSamples()) };

        m_conduitBufferDouble.clear();
        m_conduitBufferDouble.assign(static_cast<std::size_t>(size), 0.0f);
        m_conduitBuffer.clear();
        m_conduitBuffer.reserve(static_cast<std::size_t>(size));

        m_audioFifoDouble->popAudioData(m_conduitBufferDouble);

        for (const auto &d : m_conduitBufferDouble)
            m_conduitBuffer.push_back(static_cast<float>(d));

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzerCurve::LockedCurve vsa::SpectrumAnalyzer::getAnalyzerCurve()
{
    m_analyzerCurve.setSampleRate(m_audioFifoFloat ? m_audioFifoFloat->getSampleRate()
                                                   : m_audioFifoDouble->getSampleRate());

    return m_analyzerCurve.getCurve();
}

//-----------------------------------------------------------------------------
