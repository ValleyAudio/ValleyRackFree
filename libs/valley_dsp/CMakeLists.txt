cmake_minimum_required(VERSION 3.16)
project(ValleyDSP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sanitizers configuration option
option(VALLEY_ENABLE_SANITIZERS "Enable Address and Undefined Behavior Sanitizers" OFF)
if(VALLEY_ENABLE_SANITIZERS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        message(STATUS "Enabling Address and Undefined Behavior Sanitizers")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    endif()
endif()

# SIMDe header support
# Check if vendored SIMDe headers are present in third_party/simde
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/simde/simde-common.h")
    message(STATUS "Using vendored SIMDe from third_party/simde")
else()
    message(STATUS "Fetching SIMDe via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
        simde
        GIT_REPOSITORY https://github.com/simd-everywhere/simde.git
        GIT_TAG        v0.8.2
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(simde)
endif()

# Valley DSP library target
add_library(valley_dsp STATIC
    src/valley_dsp.cpp
)

target_include_directories(valley_dsp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/simde>
    $<INSTALL_INTERFACE:include>
)

add_library(valley::dsp ALIAS valley_dsp)

# Fetch Catch2 for tests
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

# Register tests
enable_testing()
add_subdirectory(tests)
