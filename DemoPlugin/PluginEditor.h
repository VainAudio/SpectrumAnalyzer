#pragma once

//-------------------------------------------------------------------------------------------------------

#include <juce_audio_processors/juce_audio_processors.h>
#include <vsa_spectrum_analyzer/vsa_spectrum_analyzer.h>

//-------------------------------------------------------------------------------------------------------

class SpectrumAnalyzerAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit SpectrumAnalyzerAudioProcessorEditor(juce::AudioProcessor &p, vsa::AudioBufferFifo<float> &source);
    ~SpectrumAnalyzerAudioProcessorEditor() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    vsa::SpectrumAnalyzerComponent m_analyzer;
};

//-------------------------------------------------------------------------------------------------------

