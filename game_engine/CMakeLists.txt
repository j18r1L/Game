cmake_minimum_required(VERSION 3.18)

project(game_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)



#add_definitions(-DHE_PLATFORM_WINDOWS, -DHE_BUILD_DLL)
add_definitions(-DHE_PLATFORM_LINUX)


include_directories(${CMAKE_SOURCE_DIR} HartEng/vendor/spdlog/include)
include_directories(${CMAKE_SOURCE_DIR} HartEng/vendor/GLFW/include)

file(GLOB source
    "HartEng/*.cpp"
)

file(GLOB header
    "HartEng/*.h"
    "HartEng/Events/*.h"
)


add_executable(game_engine ${source} ${header} main.cpp HartEng.h)

target_precompile_headers(game_engine PUBLIC "HartEng/pch.h")

target_link_libraries(game_engine Qt5::Core)

