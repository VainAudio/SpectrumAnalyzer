/**
    BEGIN_JUCE_MODULE_DECLARATION

    ID:                 exl_spectrum_analyzer
    vendor:             VainAudio
    version:            0.0.1
    name:               Exile Spectrum Analyzer
    description:        Vain Audio Spectrum Analyzer
    website:            VainAudio.com
    license:            NA
    minimumCppStandard: 20
    dependencies:       juce_dsp

    END_JUCE_MODULE_DECLARATION
 */
 
#pragma once

#include <vsa_spectrum_analyzer/SpectrumAnalyzer/SpectrumAnalyzer.h>
#include <vsa_spectrum_analyzer/SpectrumAnalyzer/SpectrumAnalyzerAverager.h>
#include <vsa_spectrum_analyzer/SpectrumAnalyzer/SpectrumAnalyzerBinSmoother.h>
#include <vsa_spectrum_analyzer/SpectrumAnalyzer/SpectrumAnalyzerBuffer.h>
#include <vsa_spectrum_analyzer/SpectrumAnalyzer/SpectrumAnalyzerComponent.h>
#include <vsa_spectrum_analyzer/SpectrumAnalyzer/SpectrumAnalyzerCurve.h>