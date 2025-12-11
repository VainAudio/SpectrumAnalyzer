set(FORMATS VST3 Standalone)

juce_add_plugin(
		VainSpectrumAnalyzer
    	COMPANY_NAME "Vain Audio"
    	COMPANY_WEBSITE "www.vainaudio.com"
		BUNDLE_ID com.VainAudio.VainAudio
		COPY_PLUGIN_AFTER_BUILD FALSE
		PLUGIN_MANUFACTURER_CODE Vain
		PLUGIN_CODE VaSA
		FORMATS "${FORMATS}"
		PRODUCT_NAME "Vain Spectrum Analyzer Demo"
)
	
set(
		SourceFiles
		PluginEditor.cpp
		PluginProcessor.cpp
)

target_sources(VainSpectrumAnalyzer PRIVATE ${SourceFiles})

target_include_directories(VainSpectrumAnalyzer PUBLIC DemoPlugin)

target_compile_definitions(
		VainSpectrumAnalyzer
    	PUBLIC
    	JUCE_WEB_BROWSER=0
    	JUCE_USE_CURL=0
    	JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(
		VainSpectrumAnalyzer

		PUBLIC

		juce::juce_core
		juce::juce_gui_basics
    	juce::juce_audio_utils
    	juce::juce_audio_processors
		juce::juce_dsp
    	juce::juce_recommended_config_flags
    	juce::juce_recommended_lto_flags
    	juce::juce_recommended_warning_flags

		vsa::vsa
		vsa::vsa_spectrum_analyzer
)
