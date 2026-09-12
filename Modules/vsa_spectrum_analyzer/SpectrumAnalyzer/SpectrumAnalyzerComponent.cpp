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

void vsa::SpectrumAnalyzerComponent::strokeAnalyzerCurve(juce::Graphics &g, const juce::Path &p)
{
    juce::ignoreUnused(g, p);
}

//-----------------------------------------------------------------------------

void vsa::SpectrumAnalyzerComponent::fillAnalyzerCurve(juce::Graphics &g, const juce::Path &p)
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

    auto mapPoint = [this, b](const juce::Point<float> &p)
    {
        const float x{ juce::jmap(p.x, 0.0f, b.getWidth()) };
        const float y{ juce::jmap(1.0f - (getPreSmoothingPathHeightMultiplier(x) * p.y), 0.0f, b.getHeight()) };

        return juce::Point{ x, y };
    };

    m_curvePath.startNewSubPath(mapPoint(levels.front()));

    for (const auto &point : levels)
    {
        m_curvePath.lineTo(mapPoint(point));
    }

    auto strokePath = m_curvePath.createPathWithRoundedCorners(50.0f);

    strokeAnalyzerCurve(g, strokePath);

    m_curvePath.swapWithPath(strokePath);
    m_curvePath.lineTo(b.getBottomRight());
    m_curvePath.lineTo(b.getBottomLeft());
    m_curvePath.closeSubPath();

    fillAnalyzerCurve(g, m_curvePath);
}

//-----------------------------------------------------------------------------
