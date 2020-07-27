cmake_minimum_required(VERSION 3.5)

project(game_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)


#add_definitions(-DHE_PLATFORM_WINDOWS, -DHE_BUILD_DLL)
add_definitions(-DHE_PLATFORM_LINUX)


file(GLOB source
    "HartEng/*.cpp"
)

file(GLOB header
    "HartEng/*.h"
)

#file(GLOB sbdlog
#    "HartEng/vendor/spdlog/*"
#)

include_directories(${CMAKE_SOURCE_DIR} HartEng/vendor/spdlog/include)

add_executable(game_engine ${source} ${header} ${sbdlog}  main.cpp HartEng.h)

#add_executable(game_engine
#  main.cpp
#  HartEng/Core.h
#  HartEng/Application.cpp
#  HartEng/Application.h
#  HartEng/HartEng.h
#)
target_link_libraries(game_engine Qt5::Core)
