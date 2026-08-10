#include "PluginEditor.h"

//-------------------------------------------------------------------------------------------------------

SpectrumAnalyzerAudioProcessorEditor::SpectrumAnalyzerAudioProcessorEditor(juce::AudioProcessor &p,
                                                                           vsa::AudioBufferFifo<float> &source)
    : juce::AudioProcessorEditor(p)
    , m_thread("fft")
    , m_analyzer(source, m_thread)
{
    m_thread.startThread();
    setSize(1000, 400);
    addAndMakeVisible(m_analyzer);
    m_analyzer.startTimerHz(60);
}

//-------------------------------------------------------------------------------------------------------

SpectrumAnalyzerAudioProcessorEditor::~SpectrumAnalyzerAudioProcessorEditor()
{
    m_thread.removeAllClients();
    m_thread.stopThread(100);
}

//-------------------------------------------------------------------------------------------------------

void SpectrumAnalyzerAudioProcessorEditor::paint(juce::Graphics &g)
{
    constexpr juce::uint8 c{ 0x11 };
    g.fillAll(juce::Colour::fromRGB(c, c, c));
}

//-------------------------------------------------------------------------------------------------------

void SpectrumAnalyzerAudioProcessorEditor::resized()
{
    m_analyzer.setBounds(getLocalBounds());
}

//-------------------------------------------------------------------------------------------------------
