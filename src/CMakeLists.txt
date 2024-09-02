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
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/char_traits.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/conversion.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/dllmain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/tinywin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/impl/utils.hpp"  
)
set(MJSTR_RES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/mjstr/res/mjstr.rc"
)

# detect if the compiler is GCC (required in next steps)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(is_gcc TRUE)
else()
    set(is_gcc FALSE)
endif()

# organize source files into directories for better readability
source_group("src" FILES ${MJSTR_INC_FILES} ${MJSTR_SRC_FILES})
source_group("src/impl" FILES ${MJSTR_IMPL_FILES})
source_group("res" FILES ${MJSTR_RES_FILES})

# add the MJSTR shared library and adjust its properties
add_library(mjstr SHARED ${MJSTR_INC_FILES} ${MJSTR_SRC_FILES} ${MJSTR_IMPL_FILES})
target_compile_definitions(mjstr PRIVATE _MJSTR_BUILD=1)
target_compile_features(mjstr PRIVATE cxx_std_20)
target_include_directories(mjstr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/mjmem/src)
target_link_libraries(mjstr PRIVATE mjmem)
set_target_properties(mjstr PROPERTIES PREFIX "") # prevent compilers from adding "lib" prefix

# Note: GCC doesn't generates LIB files, as it uses its own archive files. To maintain compatibility
#       between compilers, it must generate a LIB file. To do so, 'pexports' and 'dlltool' tools can be used,
#       the first one to generate definition file (DEF), and the second one to generate a LIB file.
if(${is_gcc} AND WIN32)
    set(target_dir "$<TARGET_FILE_DIR:mjstr>")
    add_custom_command(TARGET mjstr POST_BUILD
        COMMAND pexports ${target_dir}/mjstr.dll > ${target_dir}/mjstr.def
        COMMAND dlltool -d ${target_dir}/mjstr.def -l ${target_dir}/mjstr.lib
    )
    unset(target_dir)
endif()

# Note: RC files are used only on Windows and are compiled with rc.exe (MSVC), windres.exe (GCC),
#       or llvm-windres.exe (Clang). Therefore, they are included only when compiling for Windows.
if(WIN32)
    target_sources(mjstr PRIVATE ${MJSTR_RES_FILES})
endif()

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
    if(${is_gcc} AND WIN32)
        # copy manually generated mjstr.lib to the installation directory
        install(FILES "$<TARGET_FILE_DIR:mjstr>/mjstr.lib" DESTINATION "bin/$<CONFIG>")
    endif()
endif()