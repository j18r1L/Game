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
add_definitions(-DHE_ENABLE_ASSERTS)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/HartEng/vendor")


include_directories(${CMAKE_SOURCE_DIR} HartEng/vendor/spdlog/include)

file(GLOB source
    "HartEng/*.cpp"
    "HartEng/Platform/Linux/*.cpp"
)

file(GLOB header
    "HartEng/*.h"
    "HartEng/Events/*.h"
    "HartEng/Platform/Linux/*.h"
)


add_executable(game_engine ${source} ${header}  main.cpp HartEng.h)

target_precompile_headers(game_engine PUBLIC "HartEng/pch.h")

target_link_libraries(game_engine Qt5::Core)

# GLFW
set(GLFW_DIR "${LIB_DIR}/GLFW")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# GLAD
set(GLAD_DIR "${LIB_DIR}/GLAD")
add_subdirectory("${GLAD_DIR}")
target_link_libraries(${PROJECT_NAME} "glad" "${GLAD_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")



