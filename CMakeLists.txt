cmake_minimum_required(VERSION 3.24.1)

project(VainSpectrumAnalyzer VERSION 1.0.0)

include(FetchContent)
FetchContent_Declare(
		juce
		GIT_REPOSITORY "https://github.com/juce-framework/JUCE.git"
		GIT_TAG "8.0.10"
		SOURCE_DIR "${CMAKE_BINARY_DIR}/juce"
		BINARY_DIR "${CMAKE_BINARY_DIR}/juce_build"
)
FetchContent_MakeAvailable(juce)

add_subdirectory(Modules)
add_subdirectory(DemoPlugin)
add_subdirectory(Tests)
