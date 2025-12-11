#include "SpectrumAnalyzerBuffer.h"

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerBuffer::write(const std::span<const float> data)
{
    const auto writeSize{ data.size() };

    jassert(writeSize <= m_writeBuffer.size());
    jassert(writeSize > 0);

    const bool isTornWrite{ getWriteIndex() + writeSize >= m_writeBuffer.size() };

    const auto oldWriteIndex = static_cast<int>(getWriteIndex());

    if(isTornWrite)
    {
        const auto aBegin{ m_writeBuffer.begin() + oldWriteIndex };
        const auto aEnd{ m_writeBuffer.end() };

        const auto bBegin{ m_fftBuffer.begin() };
        const auto bEnd{ m_fftBuffer.begin() + static_cast<int>(writeSize) - std::distance(aBegin, aEnd) };

        const auto firstWriteSize = static_cast<std::size_t>(std::distance(aBegin, aEnd));
        const auto secondWriteSize = static_cast<std::size_t>(std::distance(bBegin, bEnd));

        std::ranges::copy(data.first(firstWriteSize), aBegin);
        std::ranges::copy(data.last(secondWriteSize), bBegin);
    }
    else
    {
        const auto aBegin{ m_writeBuffer.begin() + oldWriteIndex };
        const auto aEnd{ aBegin + static_cast<int>(writeSize) };

        const auto firstWriteSize = static_cast<std::size_t>(std::distance(aBegin, aEnd));

        std::ranges::copy(data.first(firstWriteSize), aBegin);
    }

    addToWriteIndex(static_cast<int>(writeSize));
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerBuffer::skip(int samples)
{
    addToWriteIndex(samples);
}

//-----------------------------------------------------------------------------

bool vsa::SpectrumAnalyzerBuffer::isFftSpanReady() const
{
    return m_isFftBufferReady;
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzerBuffer::SpectrumAnalyzerBuffer() = default;

//-----------------------------------------------------------------------------

std::size_t vsa::SpectrumAnalyzerBuffer::getWriteIndex() const
{
    return m_writeIndex;
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerBuffer::addToWriteIndex(int n)
{
    jassert(m_fftBuffer.size() == m_writeBuffer.size());
    const auto size{ m_fftBuffer.size() };

    m_writeIndex += static_cast<std::size_t>(n);

    if(m_writeIndex >= size)
    {
        m_writeIndex %= size;
        std::swap(m_writeBuffer, m_fftBuffer);
        m_isFftBufferReady = true;
    }
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzerBuffer::FftSpan vsa::SpectrumAnalyzerBuffer::getFftSpan()
{
    jassert(m_isFftBufferReady);
    m_isFftBufferReady = false;
    return m_fftBuffer;
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerBuffer::setSize(const std::size_t& size)
{
    m_writeBuffer.clear();
    m_writeBuffer.assign(size, 0.0f);
    m_fftBuffer.clear();
    m_fftBuffer.assign(size, 0.0f);
}

//-----------------------------------------------------------------------------

int vsa::SpectrumAnalyzerBuffer::getSize() const
{
    return static_cast<int>(m_writeBuffer.size());
}

//-----------------------------------------------------------------------------
