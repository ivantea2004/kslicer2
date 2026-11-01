cmake_minimum_required(VERSION 3.20)
project(kslicer2)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.30)
    cmake_policy(SET CMP0168 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM 22.1 REQUIRED CONFIG)
find_package(Clang 22.1 REQUIRED CONFIG)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/include.zip
    SOURCE_DIR external/nlohmann
)
FetchContent_Declare(
    inja
    URL https://github.com/pantor/inja/releases/download/v3.5.0/inja.hpp
    SOURCE_DIR external/inja/include/inja
    DOWNLOAD_NO_EXTRACT ON
)

FetchContent_MakeAvailable(nlohmann_json)
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json SYSTEM INTERFACE ${nlohmann_json_SOURCE_DIR}/single_include)

FetchContent_MakeAvailable(inja)
add_library(inja INTERFACE)
target_include_directories(inja SYSTEM INTERFACE ${inja_SOURCE_DIR}/..)
target_link_libraries(inja INTERFACE nlohmann_json)

add_executable(kslicer2
    src/main.cpp
    src/io.cpp
    src/utils.cpp
    src/errors.cpp
    src/codegen_host.cpp
    src/class_info.cpp
    src/codegen.cpp
    src/compiler.cpp
    src/command_line.cpp
    src/command_line_parser.cpp
    src/config.cpp
    src/kslicer_cpp_attributes.cpp
    src/template_rendering.cpp
)

# Imported target LLVMDebugInfoPDB wants to link with a library which probably wouldn't be present.
# Clearing INTERFACE_LINK_LIBRARIES does not break linking, so that library is not need anyway.
set_target_properties(LLVMDebugInfoPDB PROPERTIES INTERFACE_LINK_LIBRARIES "")

if(WIN32 AND(NOT MINGW))
    # When linking with clang static libraries on Windows this macro should be defined for macro CLANG_ABI to be defined correctly.
    # Not doing so can cause linking warnings (and possible silent errors)
    # Normally, this macro is added during CMake configuration of clang/llvm project and then propagated through imported targets in ClangTargets.cmake,
    # but if clang/llvm is built on Windows not by msvc but by clang then this macro is not defined.
    # It is a bug and it is already fixed in llvm 23.0.
    target_compile_definitions(kslicer2 PRIVATE CLANG_BUILD_STATIC)
endif()

if(MSVC)
    target_compile_options(kslicer2 PRIVATE /FI ${CMAKE_CURRENT_SOURCE_DIR}/src/do_not_use_printf_and_cout_use_kslicer_outs_intead.h)
else()
    target_compile_options(kslicer2 PRIVATE -include ${CMAKE_CURRENT_SOURCE_DIR}/src/do_not_use_printf_and_cout_use_kslicer_outs_intead.h)
endif()

target_include_directories(kslicer2 SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
target_link_directories(kslicer2 PRIVATE ${LLVM_LIBRARY_DIRS})
target_link_libraries(kslicer2 PRIVATE clangTooling inja)

if(MSVC)
    target_compile_options(kslicer2 PRIVATE /W4)

    # Disabling this warnings because even when included as SYSTEM (/external) it still produces warnings inside llvm headers.
    # Warnings C47xx are not affected by SYSTEM (/external).
    # https://learn.microsoft.com/en-us/cpp/build/reference/external-external-headers-diagnostics?view=msvc-170#limitations
    target_compile_options(kslicer2 PRIVATE /wd4702)
else()
    target_compile_options(kslicer2 PRIVATE -Wall -Wextra)
endif()
