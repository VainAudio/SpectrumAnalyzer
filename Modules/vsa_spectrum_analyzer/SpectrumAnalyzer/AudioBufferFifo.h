#pragma once

//-------------------------------------------------------------------------------------------------------

#include <juce_dsp/juce_dsp.h>
#include <span>

//--------------------------------------------------------------------------------

BEGIN_VSA_NAMESPACE

//--------------------------------------------------------------------------------
/**
 * @class AudioBufferFifo
 * @brief This is the part that lives in the processor and accepts new audio data
 *
 * audio thread only: setSize(), pushAudioData()
 * ui thread only: popAudioData()
 *
 * a lock ensures the vector cannot be resized while audio is being read.
 * it does note exist while audio is being written - writing and resizing should
 * happen only on the same thread.
*/
template<typename SampleType>
class AudioBufferFifo
{
public:
    AudioBufferFifo() = default;

    void setSize(double sampleRate, double lengthInSeconds)
    {
        m_sampleRate = sampleRate;
        const int length{ static_cast<int>(sampleRate * lengthInSeconds) };
        std::lock_guard<std::mutex> guard{ m_fifoResizeLock };

        m_abstractFifo.setTotalSize(length);
        m_audioFifo.clear();
        m_audioFifo.insert(m_audioFifo.begin(), static_cast<std::size_t>(length), 0.0f);
    }
    void pushAudioData(const juce::AudioBuffer<SampleType> &buffer)
    {
        jassert(!m_audioFifo.empty()); // call setSize

        const juce::AbstractFifo::ScopedWrite write{ m_abstractFifo, buffer.getNumSamples() };

        if(write.blockSize1 > 0)
        {
            const SampleType * const dataRead{ buffer.getReadPointer(0) };
            const std::span<const SampleType> inSpan{ dataRead, dataRead + write.blockSize1 };

            const auto outBegin{ m_audioFifo.begin() + write.startIndex1 };
            std::ranges::copy(inSpan, outBegin);
        }

        if(write.blockSize2 > 0)
        {
            const SampleType * const dataRead{ buffer.getReadPointer(0) + write.blockSize1 };
            const std::span<const SampleType> inSpan{ dataRead, dataRead + write.blockSize2 };

            const auto outBegin{ m_audioFifo.begin() + write.startIndex2 };
            std::ranges::copy(inSpan, outBegin);
        }
    }

    void pushSample(const SampleType &sample)
    {
        juce::AbstractFifo::ScopedWrite write{ m_abstractFifo, 1 };
        if(write.blockSize1 > 0)
            m_audioFifo[static_cast<std::size_t>(write.startIndex1)] = sample;

        jassert(write.blockSize2 == 0);
    }

    void popAudioData(std::span<SampleType> dest)
    {
        std::lock_guard<std::mutex> guard{ m_fifoResizeLock };

        const juce::AbstractFifo::ScopedRead read{ m_abstractFifo, static_cast<int>(dest.size()) };

        if(read.blockSize1 > 0)
        {
            std::span<SampleType> readSpan{ m_audioFifo.begin() + read.startIndex1,
                                       m_audioFifo.begin() + read.startIndex1 + read.blockSize1 };

            std::ranges::copy(readSpan, dest.begin());
        }

        if(read.blockSize2 > 0)
        {
            std::span<SampleType> readSpan{ m_audioFifo.begin() + read.startIndex2,
                                       m_audioFifo.begin() + read.startIndex2 + read.blockSize2 };

            std::ranges::copy(readSpan, dest.begin() + read.blockSize1);
        }
    }

    int getAvailableSamples() const
    {
        return m_abstractFifo.getNumReady();
    }

    double getSampleRate() const
    {
        return m_sampleRate;
    }

private:
    double m_sampleRate{};
    std::mutex m_fifoResizeLock;
    juce::AbstractFifo m_abstractFifo{ 1 };
    std::vector<SampleType> m_audioFifo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioBufferFifo)
};

//--------------------------------------------------------------------------------

END_VSA_NAMESPACE

//--------------------------------------------------------------------------------
