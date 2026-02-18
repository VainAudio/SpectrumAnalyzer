#include "SpectrumAnalyzerComponent.h"
#include "SpectrumAnalyzer.h"

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzerComponent::SpectrumAnalyzerComponent(AudioBufferFifo<float> &source, juce::TimeSliceThread &thread, int fftSize)
    : m_fft(source, thread, fftSize)
{
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzerComponent::SpectrumAnalyzerComponent(AudioBufferFifo<double> &source, juce::TimeSliceThread &thread, int fftSize)
    : m_fft(source, thread, fftSize)
{
}

//-----------------------------------------------------------------------------

vsa::SpectrumAnalyzerComponent::~SpectrumAnalyzerComponent()
{
    stopTimer();
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerComponent::timerCallback()
{
    repaint();
}

//-----------------------------------------------------------------------------

float vsa::SpectrumAnalyzerComponent::getPreSmoothingPathHeightMultiplier(float x)
{
    juce::ignoreUnused(x);
    return 1.0f;
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerComponent::paintAnalyzerCurve(juce::Graphics &g, const juce::Path &p)
{
    const auto gradient{ juce::ColourGradient::vertical(juce::Colours::red, 0.0f, juce::Colours::red.withAlpha(0.25f),
                                                        static_cast<float>(getHeight())) };
    g.setGradientFill(gradient);

    g.fillPath(p);
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerComponent::paint(juce::Graphics &g)
{
    m_curvePath.clear();

    const auto b{ getLocalBounds().toFloat() };
    const auto levels{ m_fft.getAnalyzerCurve() };

    const auto bottomLeft{ b.getBottomLeft() };
    m_curvePath.startNewSubPath(bottomLeft);

    for (const auto &point : levels)
    {
        const float x{ juce::jmap(point.x, 0.0f, b.getWidth()) };
        const float y{ juce::jmap(1.0f - (getPreSmoothingPathHeightMultiplier(x) * point.y), 0.0f, b.getHeight()) };

        m_curvePath.lineTo(juce::Point{ x, y });
    }

    m_curvePath = m_curvePath.createPathWithRoundedCorners(50.0f);

    const auto bottomRight{ b.getBottomRight() };
    m_curvePath.lineTo(bottomRight);

    paintAnalyzerCurve(g, m_curvePath);
}

//-----------------------------------------------------------------------------
