# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)
project(mjstr_benchmarks LANGUAGES CXX)

# build Google Benchmark before building any benchmarks
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(google-benchmark "${CMAKE_CURRENT_BINARY_DIR}/google-benchmark" EXCLUDE_FROM_ALL)
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # suppress -Winvalid-offsetof warning to compile Google Benchmark with Clang on Windows
    target_compile_options(benchmark PRIVATE -Wno-invalid-offsetof)
endif()

function(add_isolated_benchmark benchmark_name benchmark_path)
    # create an executable for the specified benchmark, linking against necessary libraries
    add_executable(${benchmark_name} ${benchmark_path})
    target_compile_definitions(${benchmark_name} PRIVATE BENCHMARK_STATIC_DEFINE)
    target_compile_features(${benchmark_name} PRIVATE cxx_std_20)
    target_include_directories(${benchmark_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/mjmem/src"
        "${benchmark_SOURCE_DIR}/include"
    )
    target_link_libraries(${benchmark_name} PRIVATE benchmark mjmem mjstr)
    if(WIN32)
        # link against Shlwapi.lib, a Windows-specific statically-linked library
        target_link_libraries(${benchmark_name} PRIVATE Shlwapi)
    endif()
endfunction()

add_isolated_benchmark(benchmark_conversion "src/conversion/benchmark.cpp")

# use a custom target to combine all targets into a single one,
# this allows only one post-build call instead of per-benchmark copying
add_custom_target(mjstr_and_benchmarks ALL DEPENDS
    mjstr
    mjmem # register dependencies as well
    benchmark_conversion
)
add_custom_command(TARGET mjstr_and_benchmarks POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:mjmem>"
    "$<TARGET_FILE:mjstr>"
    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>" # copy to the benchmarks' output directory
)