#pragma once

//-----------------------------------------------------------------------------

#include <vsa/vsa.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "SpectrumAnalyzer.h"

//-----------------------------------------------------------------------------

BEGIN_VSA_NAMESPACE

//-----------------------------------------------------------------------------
/**
 * @class SpectrumAnalyzerComponent
 * @brief Display a spectrum analyzer from data grabbed from AudioBufferFifo
 *
 * @see AudioBufferFifo
 */
class SpectrumAnalyzerComponent
    : public juce::Component
    , public juce::Timer
{
public:
    explicit SpectrumAnalyzerComponent(AudioBufferFifo<float> &source, juce::TimeSliceThread &thread, int fftSize = 11);
    explicit SpectrumAnalyzerComponent(AudioBufferFifo<double> &source, juce::TimeSliceThread &thread, int fftSize = 11);
    ~SpectrumAnalyzerComponent() override;

    void paint(juce::Graphics &g) override;

private:
    // juce::Timer
    void timerCallback() final;

    virtual float getPreSmoothingPathHeightMultiplier(float x);
    virtual void paintAnalyzerCurve(juce::Graphics &g, const juce::Path &p);

    SpectrumAnalyzer m_fft;
    juce::Path m_curvePath;
    const juce::Colour m_colour{ juce::Colours::red };
};

//-----------------------------------------------------------------------------

END_VSA_NAMESPACE

//-----------------------------------------------------------------------------
