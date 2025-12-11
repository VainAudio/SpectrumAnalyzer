# Vain Spectrum Analyzer

![alt text](demo.webp "Demo")

A JUCE module containing the spectrum analyzer code used in [Sentinel](https://vainaudio.com/product/sentinel/), [PFT](https://vainaudio.com/product/pft/), and [Orange Gate](https://vainaudio.com/product/orange-gate/).

## Basic Usage
A basic spectrum analyzer is accomplished using two classes: `vsa::AudioBufferFifo` and `vsa::SpectrumAnalyzerComponent`.
The `vsa::AudioBufferFifo` lives wherever audio is being processed.
The `vsa::SpectrumAnalyzerComponent` lives in the message thread and regularly polls the fifo from a dedicated thread for new audio data.

```c++
vsa::AudioBufferFifo fifo;
fifo.setSize(sampleRate, 1.0); // store 1 second of data

juce::AudioBuffer<float> buffer;
m_fifo.pushAudioData(buffer); // push all audio data into the fifo

// message thread
vsa::SpectrumAnalyzerComponent spectrumAnalyzer{ fifo };
addAndMakeVisible(spectrumAnalyzer);
```
