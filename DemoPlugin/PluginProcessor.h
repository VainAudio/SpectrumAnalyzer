#pragma once

//-------------------------------------------------------------------------------------------------------

#include <juce_audio_processors/juce_audio_processors.h>
#include <vsa_spectrum_analyzer/vsa_spectrum_analyzer.h>

//-------------------------------------------------------------------------------------------------------

class SpectrumAnalyzerAudioProcessor : public juce::AudioProcessor
{
public:
    SpectrumAnalyzerAudioProcessor();
    ~SpectrumAnalyzerAudioProcessor() override;

    const juce::String getName() const override;
    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;
    double getTailLengthSeconds() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    juce::AudioProcessorEditor * createEditor() override;
    bool hasEditor() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

private:
    vsa::AudioBufferFifo<float> m_fifo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzerAudioProcessor)
};

//-------------------------------------------------------------------------------------------------------
