enable_testing()

FetchContent_Declare(
		gtest
		GIT_REPOSITORY "https://github.com/google/googletest.git"
		GIT_TAG "main"
		SOURCE_DIR "${CMAKE_BINARY_DIR}/gtest"
		BINARY_DIR "${CMAKE_BINARY_DIR}/gtest_build"
)
FetchContent_MakeAvailable(gtest)

add_executable(Tests Tests.cpp)

target_compile_definitions(
		Tests
		PUBLIC
		JUCE_WEB_BROWSER=0
		JUCE_USE_CURL=0
)

target_link_libraries(
		Tests

		PUBLIC

		gtest

		vsa::vsa
		vsa::vsa_spectrum_analyzer

		juce::juce_core
		juce::juce_gui_basics
		juce::juce_audio_utils
		juce::juce_audio_processors
		juce::juce_dsp
		juce::juce_recommended_config_flags
		juce::juce_recommended_lto_flags
		juce::juce_recommended_warning_flags
)