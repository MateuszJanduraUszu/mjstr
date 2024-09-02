# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)
project(mjstr_tests LANGUAGES CXX)

# build Google Test as a static library before building any tests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(google-test "${CMAKE_CURRENT_BINARY_DIR}/google-test" EXCLUDE_FROM_ALL)

function(add_isolated_test test_name test_path)
    # create an executable for the specified test, linking against necessary libraries
    add_executable(${test_name} ${test_path})
    target_compile_features(${test_name} PRIVATE cxx_std_20)
    target_include_directories(${test_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/mjmem/src"
        "${gtest_SOURCE_DIR}/include"
    )
    target_link_libraries(${test_name} PRIVATE gtest gtest_main mjmem mjstr)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_isolated_test(test_char_traits "src/char_traits/test.cpp")
add_isolated_test(test_conversion "src/conversion/test.cpp")
add_isolated_test(test_string "src/string/test.cpp")
add_isolated_test(test_string_iterator "src/string_iterator/test.cpp")
add_isolated_test(test_string_view "src/string_view/test.cpp")
add_isolated_test(test_string_view_iterator "src/string_view_iterator/test.cpp")

# use a custom target to combine all targets into a single one,
# this allows only one post-build call instead of per-test copying
add_custom_target(mjstr_and_tests ALL DEPENDS
    mjstr
    mjmem # register dependencies as well
    test_char_traits
    test_conversion
    test_string
    test_string_iterator
    test_string_view
    test_string_view_iterator
)
add_custom_command(TARGET mjstr_and_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:mjmem>"
    "$<TARGET_FILE:mjstr>"
    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>" # copy to the tests' output directory
)