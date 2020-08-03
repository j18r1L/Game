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
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
add_definitions(-DHE_TRACE_EVENTS)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/HartEng/vendor")

#imGUI
set(imGUI_DIR "${LIB_DIR}/imGUI")
include_directories(${imGUI_DIR})

file(GLOB source
    "HartEng/*.cpp"
    "HartEng/Platform/Linux/*.cpp"
    "HartEng/Platform/OpenGL/*.cpp"
    "HartEng/imGUI/*.cpp"
    "HartEng/Renderer/*.cpp"

    # Временно
    "HartEng/vendor/imGUI/*.cpp"
    "HartEng/vendor/imGUI/examples/imgui_impl_opengl3.cpp"
    "HartEng/vendor/imGUI/examples/imgui_impl_glfw.cpp"

)

file(GLOB header
    "HartEng/*.h"
    "HartEng/Events/*.h"
    "HartEng/Platform/Linux/*.h"
    "HartEng/Platform/OpenGL/*.h"
    "HartEng/imGUI/*.h"
    "HartEng/Renderer/*.h"

    # Временно
    "HartEng/vendor/imGUI/*.h"
    "HartEng/vendor/imGUI/examples/imgui_impl_opengl3.h"
    "HartEng/vendor/imGUI/examples/imgui_impl_glfw.h"
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

# GLM
set(GLM_DIR "${LIB_DIR}/GLM")
add_subdirectory("${GLM_DIR}")
target_link_libraries(${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")

#spdlog
set(SPDLOG_DIR "${LIB_DIR}/spdlog")
add_subdirectory("${SPDLOG_DIR}")
target_link_libraries(${PROJECT_NAME} "spdlog" "${SPDLOG_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SPDLOG_DIR}/include")





