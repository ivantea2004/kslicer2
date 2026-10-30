cmake_minimum_required(VERSION 3.20)
project(kslicer2)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM 23.1 REQUIRED CONFIG)
find_package(Clang 23.1 REQUIRED CONFIG)

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

add_subdirectory(kslicer2)
