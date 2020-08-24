cmake_minimum_required(VERSION 3.16)

project(game_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform
if (WIN32)
    add_definitions(-DHE_PLATFORM_WINDOWS)
endif (WIN32)

if (UNIX)
    add_definitions(-DHE_PLATFORM_LINUX)
    add_definitions(-DHE_QT_CTEATOR)
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
add_definitions(-DHE_TRACE_EVENTS)

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

file(GLOB source
    "HartEng/Core/*.cpp"
    "HartEng/*.cpp"

    "HartEng/imGUI/*.cpp"

    "HartEng/Renderer/*.cpp"

    "HartEng/Platform/Linux/*.cpp"
    "HartEng/Platform/OpenGL/*.cpp"

    "HartEng/Debug/*.h"

    "HartEng/vendor/stb/*.cpp"
    "HartEng/vendor/imGUI/*.cpp"
    "HartEng/vendor/imGUI/examples/imgui_impl_opengl3.cpp"
    "HartEng/vendor/imGUI/examples/imgui_impl_glfw.cpp"

)

file(GLOB header
    "HartEng/Core/*.h"
    "HartEng/*.h"
    "HartEng/Events/*.h"

    "HartEng/imGUI/*.h"

    "HartEng/Renderer/*.h"

    "HartEng/Platform/Linux/*.h"
    "HartEng/Platform/OpenGL/*.h"

    "HartEng/Debug/*.h"

    "HartEng/vendor/stb/*.h"
    "HartEng/vendor/imGUI/*.h"
    "HartEng/vendor/imGUI/examples/imgui_impl_opengl3.h"
    "HartEng/vendor/imGUI/examples/imgui_impl_glfw.h"
)

file(GLOB main_project
    "*.cpp"
    "*.h"
)


add_executable(game_engine ${source} ${header} ${main_project} HartEng.h main.cpp )

target_precompile_headers(game_engine PUBLIC "HartEng/pch.h")

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





