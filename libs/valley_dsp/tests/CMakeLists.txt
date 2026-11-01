cmake_minimum_required(VERSION 3.16)

set(AUDIO_REFERENCE_DSP_SOURCES
    ${PROJECT_SOURCE_DIR}/../../src/dsp/filters/OnePoleFilters.cpp
    ${PROJECT_SOURCE_DIR}/../../src/dsp/filters/OTAFilter.cpp
    ${PROJECT_SOURCE_DIR}/../../src/Plateau/Dattorro.cpp
    ${PROJECT_SOURCE_DIR}/../../src/Amalgam/VecAmalgam.cpp
    ${PROJECT_SOURCE_DIR}/../../src/dsp/filters/VecOnePoleFilters.cpp
    ${PROJECT_SOURCE_DIR}/../../src/dsp/fx/VecDiodeRingMod.cpp
    ${PROJECT_SOURCE_DIR}/../../src/utilities/Utilities.cpp
)

set(AUDIO_REFERENCE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/shim
    ${PROJECT_SOURCE_DIR}/../../src
    ${PROJECT_SOURCE_DIR}/third_party
    ${PROJECT_SOURCE_DIR}/third_party/simde
    ${PROJECT_SOURCE_DIR}/include
)

# Reference Audio Generator binary
# Note: NDEBUG is defined to match Valley's Makefile behavior, disabling broken legacy asserts in src/
add_executable(audio_reference_generator
    AudioReferenceGenerator.cpp
    ${AUDIO_REFERENCE_DSP_SOURCES}
)
target_include_directories(audio_reference_generator PRIVATE ${AUDIO_REFERENCE_INCLUDES})
target_compile_definitions(audio_reference_generator PRIVATE
    NDEBUG
    REFERENCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/reference"
)

# Audio Reference Catch2 Test Runner
add_executable(test_audio_reference
    test_audio_reference.cpp
    ${AUDIO_REFERENCE_DSP_SOURCES}
)
target_include_directories(test_audio_reference PRIVATE ${AUDIO_REFERENCE_INCLUDES})
target_compile_definitions(test_audio_reference PRIVATE
    NDEBUG
    REFERENCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/reference"
)
target_link_libraries(test_audio_reference PRIVATE Catch2::Catch2WithMain)

add_test(NAME test_audio_reference COMMAND test_audio_reference)
set_tests_properties(test_audio_reference PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Custom target to explicitly regenerate reference audio snapshots
add_custom_target(generate_reference_snapshots
    COMMAND audio_reference_generator "${CMAKE_CURRENT_SOURCE_DIR}/reference"
    DEPENDS audio_reference_generator
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating audio reference snapshots in ${CMAKE_CURRENT_SOURCE_DIR}/reference"
)
