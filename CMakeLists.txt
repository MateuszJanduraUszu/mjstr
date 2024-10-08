# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)
project(mjstr_library LANGUAGES CXX)

# check if the platform architecture is supported (MJX_PLATFORM_ARCH defined in presets)
if(NOT ${MJX_PLATFORM_ARCH} MATCHES "^x64$|^x86$")
    message(FATAL_ERROR "The MJSTR library can only be built for x64 and x86 architectures.")
endif()

option(MJSTR_BUILD_DEPENDENCIES "Build MJSTR library dependencies" OFF)
option(MJSTR_BUILD_BENCHMARKS "Build MJSTR library benchmarks" OFF)
option(MJSTR_BUILD_TESTS "Build MJSTR library tests" OFF)
option(MJSTR_INSTALL_LIBRARY "Install MJSTR library" OFF)

# build the MJSTR library before building any benchmarks and tests
add_subdirectory(src "${CMAKE_CURRENT_BINARY_DIR}/library")

if(MJSTR_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks "${CMAKE_CURRENT_BINARY_DIR}/benchmarks")
endif()

if(MJSTR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests "${CMAKE_CURRENT_BINARY_DIR}/tests")
endif()