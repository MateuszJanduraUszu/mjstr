# CMakeLists.txt

# Copyright (c) Mateusz Jandura. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21.0)

# build the dependencies before building the MJSTR library
if(MJSTR_BUILD_DEPENDENCIES)
    # install all dependencies if the MJSTR is being installed
    if(MJSTR_INSTALL_LIBRARY)
        set(MJMEM_INSTALL_LIBRARY ON)
    endif()

    add_subdirectory(mjmem "${CMAKE_BINARY_DIR}/deps/mjmem")
endif()

set(MJSTR_INC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/api.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/char_traits.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/conversion.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/string.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/string_view.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/version.hpp"
)
set(MJSTR_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/char_traits.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/conversion.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/string.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/string_view.cpp"
)
set(MJSTR_IMPL_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/conversion.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/tinywin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/utils.hpp"
)
set(MJSTR_RES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/res/mjstr.rc"
)

# organize source files into directories for better readability
source_group("src" FILES ${MJSTR_INC_FILES} ${MJSTR_SRC_FILES})
source_group("src/impl" FILES ${MJSTR_IMPL_FILES})
source_group("res" FILES ${MJSTR_RES_FILES})

# add the MJSTR shared library and adjust its properties
add_library(mjstr SHARED ${MJSTR_INC_FILES} ${MJSTR_SRC_FILES} ${MJSTR_IMPL_FILES} ${MJSTR_RES_FILES})
target_compile_definitions(mjstr PRIVATE _MJSTR_BUILD=1)
target_compile_features(mjstr PRIVATE cxx_std_20)
target_include_directories(mjstr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/mjmem/src)
target_link_libraries(mjstr PRIVATE mjmem)

if(MJSTR_INSTALL_LIBRARY)
    if(NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        # set the install prefix to the default path
        set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Path to install directory" FORCE)
    endif()

    install(TARGETS mjstr mjmem
        EXPORT mjstr-config
        RUNTIME DESTINATION "bin/$<CONFIG>"
        LIBRARY DESTINATION "bin/$<CONFIG>"
        ARCHIVE DESTINATION "bin/$<CONFIG>"
    )
    install(EXPORT mjstr-config
        DESTINATION "cmake/mjstr"
        NAMESPACE mjx::
    )
    install(FILES ${MJSTR_INC_FILES} DESTINATION "inc/mjstr")
endif()