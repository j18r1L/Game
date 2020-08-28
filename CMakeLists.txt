cmake_minimum_required(VERSION 3.11)

project(game_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DCMAKE_PATH="${CMAKE_SOURCE_DIR}")



# Platform
if (WIN32)
    add_definitions(-DHE_PLATFORM_WINDOWS)
endif (WIN32)

if (UNIX)
    add_definitions(-DHE_PLATFORM_LINUX)
    add_definitions(-DHE_QT_CTEATOR)
    add_definitions(-DHE_API_OPENGL)
endif (UNIX)


if (MSVC)
    add_definitions(-DHE_VISUAL_STUDIO)
endif (MSVC)

#add_definitions(-DHE_PLATFORM_WINDOWS, -DHE_BUILD_DLL)
#add_definitions(-DHE_PLATFORM_LINUX)
#add_definitions(-DHE_PLATFORM_WINDOWS)

# IDE
#add_definitions(-DHE_VISUAL_STUDIO)
add_definitions(-DHE_QT_CTEATOR)

# Silent asserts
#add_definitions(-DHE_SILENT_ASSERTS)

# Debug
add_definitions(-DHE_DEBUG)

# Profile
add_definitions(-DHE_PROFILE)

# Event's trace
#add_definitions(-DHE_TRACE_EVENTS)

#using glad if OpenGL
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/HartEng/vendor")

# GLFW
set(GLFW_DIR "${LIB_DIR}/GLFW")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")

# GLAD
set(GLAD_DIR "${LIB_DIR}/GLAD")
add_subdirectory("${GLAD_DIR}")

# GLM
set(GLM_DIR "${LIB_DIR}/GLM")
add_subdirectory("${GLM_DIR}")

# spdlog
set(SPDLOG_DIR "${LIB_DIR}/spdlog")
add_subdirectory("${SPDLOG_DIR}")

# imGUI
set(imGUI_DIR "${LIB_DIR}/imGUI")
include_directories(${imGUI_DIR})

# entt
set(ENTT_DIR "${LIB_DIR}/entt")
include_directories("${ENTT_DIR}/include/")

# stb
set(STB_DIR "${LIB_DIR}/stb")
include_directories("${STB_DIR}")

file(GLOB game_engine_files
    "HartEng/HartEng.h"

    "HartEng/Core/*.h"
    "HartEng/Core/*.cpp"

    "HartEng/Events/*.h"

    "HartEng/imGUI/*.h"
    "HartEng/imGUI/*.cpp"

    "HartEng/Renderer/*.h"
    "HartEng/Renderer/*.cpp"

    "HartEng/Platform/Linux/*.h"
    "HartEng/Platform/Linux/*.cpp"

    "HartEng/Platform/OpenGL/*.h"
    "HartEng/Platform/OpenGL/*.cpp"

    "HartEng/Debug/*.h"
    "HartEng/Debug/*.cpp"

    "HartEng/vendor/stb/*.h"
    "HartEng/vendor/stb/*.cpp"

    "HartEng/vendor/imGUI/*.h"
    "HartEng/vendor/imGUI/*.cpp"

    "HartEng/vendor/imGUI/examples/imgui_impl_opengl3.h"
    "HartEng/vendor/imGUI/examples/imgui_impl_opengl3.cpp"

    "HartEng/vendor/imGUI/examples/imgui_impl_glfw.h"
    "HartEng/vendor/imGUI/examples/imgui_impl_glfw.cpp"

)

file(GLOB level_editor_files
    "levelEditor/*.cpp"
    "levelEditor/*.h"
)

file(GLOB sandbox_files
    "sandBox/*.cpp"
    "sandBox/*.h"
)

# CMake instruction to make static library
ADD_LIBRARY(game_engine STATIC ${game_engine_files} )

# CMake instructions to test using the static lib
SET(LEVEL_EDITOR_EXE LevelEditor)
SET(SANDBOX_EXE SandBox)

add_executable(${LEVEL_EDITOR_EXE} ${level_editor_files})
TARGET_LINK_LIBRARIES(${LEVEL_EDITOR_EXE} game_engine)

add_executable(${SANDBOX_EXE} ${sandbox_files})
TARGET_LINK_LIBRARIES(${SANDBOX_EXE} game_engine)

#target_precompile_headers(game_engine PUBLIC "HartEng/pch.h")

# GLFW
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# GLAD
target_link_libraries(${PROJECT_NAME} "glad" "${GLAD_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")

# GLM
target_link_libraries(${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")

# spdlog
target_link_libraries(${PROJECT_NAME} "spdlog" "${SPDLOG_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SPDLOG_DIR}/include")





