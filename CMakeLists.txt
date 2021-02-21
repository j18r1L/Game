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


# IDE
if (MSVC)
    add_definitions(-DHE_VISUAL_STUDIO)
    #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    #set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif (MSVC)

add_definitions(-DHE_QT_CTEATOR)

# Silent asserts
#add_definitions(-DHE_SILENT_ASSERTS)

# Debug
add_definitions(-DHE_DEBUG)

# Profile
#add_definitions(-DHE_PROFILE)

# Event's trace
#add_definitions(-DHE_TRACE_EVENTS)

#using glad if OpenGL
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/HartEng/vendor")

######## CMAKE LIBS ##########
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

# assimp
set(ASSIMP_DIR "${LIB_DIR}/Assimp")
add_subdirectory("${ASSIMP_DIR}")

# yaml
set(YAML_DIR "${LIB_DIR}/yaml")
#set(YAML_CPP_BUILD_TOOLS ON CACHE INTERNAL "Generate tools")
#set(YAML_BUILD_SHARED_LIBS ON CACHE INTERNAL "Generate shared libs")
#set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "Build tests")
add_subdirectory("${YAML_DIR}")


# PhysX submodule
#### Build PhysX library ####
# PHYSX_PATH - path to the `{cloned repository}/physx` repo directory git://github.com/NVIDIAGameWorks/PhysX.git
set( PHYSX_ROOT_DIR "${LIB_DIR}/PhysX/physx" ) #This is needed for $ENV{PHYSX_PATH}/compiler/public/CMakeLists.txt
set( PHYSX_INCLUDE_DIRS ${PHYSX_ROOT_DIR}/include/ ${PHYSX_ROOT_DIR}/../pxshared/include/ )
set( PHYSX_LIBRARIES
	PhysXExtensions
	PhysX
	PhysXPvdSDK
	PhysXVehicle
	PhysXCharacterKinematic
	PhysXCooking
	PhysXCommon
	PhysXFoundation
	# SnippetUtils
)

set(TARGET_BUILD_PLATFORM "windows") # has to match the TARGET_BUILD_PLATFORM in $ENV{PHYSX_PATH}/physix/buildtools/cmake_generate_projects.py
set(PX_BUILDSNIPPETS OFF CACHE BOOL "Generate the snippets")
set(PX_BUILDPUBLICSAMPLES OFF CACHE BOOL "Generate the samples projects")
set(PX_GENERATE_STATIC_LIBRARIES ON CACHE BOOL "Generate static libraries")
set(PX_FLOAT_POINT_PRECISE_MATH OFF CACHE BOOL "Float point precise math")
set(NV_USE_STATIC_WINCRT ON CACHE BOOL "Use the statically linked windows CRT")
set(NV_USE_DEBUG_WINCRT ON CACHE BOOL "Use the debug version of the CRT")
set(PXSHARED_PATH ${PHYSX_ROOT_DIR}/../pxshared)
set(PXSHARED_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CMAKEMODULES_VERSION "1.27")
set(CMAKEMODULES_PATH ${PHYSX_ROOT_DIR}/../externals/cmakemodules)
set(PX_OUTPUT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/HartEng/vendor/PhysX)
set(PX_OUTPUT_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/HartEng/vendor/PhysX)
  
# Call into PhysX's CMake scripts
add_subdirectory(${PHYSX_ROOT_DIR}/compiler/public externals/physx)





########### NO CMAKE LIBS ############
# imGUI
set(imGUI_DIR "${LIB_DIR}/imGUI")
include_directories(${imGUI_DIR})

# stb
set(STB_DIR "${LIB_DIR}/stb")
include_directories("${STB_DIR}")

# PhysX
#add_definitions(-DPX_PHYSX_STATIC_LIB)
#set(PHYSX_DIR "${LIB_DIR}/PhysX")
#include_directories("${PHYSX_DIR}/include")
#link_directories("${PHYSX_DIR}/lib/Debug")



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

    "HartEng/Platform/Windows/*.h"
    "HartEng/Platform/Windows/*.cpp"

    "HartEng/Platform/OpenGL/*.h"
    "HartEng/Platform/OpenGL/*.cpp"

    "HartEng/Physics/*.h"
    "HartEng/Physics/*.cpp"

    "HartEng/Asset/*.h"
    "HartEng/Asset/*.cpp"

    "HartEng/Debug/*.h"
    "HartEng/Debug/*.cpp"

    "HartEng/vendor/stb/*.h"
    "HartEng/vendor/stb/*.cpp"

    "HartEng/vendor/imGUI/*.h"
    "HartEng/vendor/imGUI/*.cpp"

    "HartEng/vendor/imGUI/backends/imgui_impl_opengl3.h"
    "HartEng/vendor/imGUI/backends/imgui_impl_opengl3.cpp"

    "HartEng/vendor/imGUI/backends/imgui_impl_glfw.h"
    "HartEng/vendor/imGUI/backends/imgui_impl_glfw.cpp"

    "HartEng/Scene/*.h"
    "HartEng/Scene/*.cpp"

    "HartEng/Scene/Components/*.h"
    "HartEng/Scene/Components/*.cpp"
)

file(GLOB level_editor_files
    "levelEditor/*.h" 
    "levelEditor/*.cpp"

    "levelEditor/Panels/*.h"
    "levelEditor/Panels/*.cpp"

    "levelEditor/Gizmo/*.h"
    "levelEditor/Gizmo/*.cpp"

    "assets/scripts/*.h"
    "assets/scripts/*.cpp"
)

file(GLOB sandbox_files
    "sandBox/*.h"
    "sandBox/*.cpp"
    

    "sandBox/Menu/*.h"
    "sandBox/Menu/*.cpp"

    "assets/scripts/*.h"
    "assets/scripts/*.cpp"

    
)

# CMake instruction to make static library
ADD_LIBRARY(game_engine STATIC ${game_engine_files} )

# CMake instructions to test using the static lib
SET(LEVEL_EDITOR_EXE LevelEditor)
SET(SANDBOX_EXE SandBox)

add_executable(${LEVEL_EDITOR_EXE} ${level_editor_files})
if (MSVC)
    #add_compile_options(  $<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd>  $<$<CONFIG:Release>:/MT> )
    #set_property(TARGET game_engine PROPERTY MSVC_RUNTIME_LIBRARY "/MTd")
endif (MSVC)
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

# assimp
target_link_libraries(${PROJECT_NAME} "assimp" "${ASSIMP_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${ASSIMP_DIR}/include")

# yaml
target_link_libraries(${PROJECT_NAME} "yaml-cpp")
target_include_directories(${PROJECT_NAME} PRIVATE "${YAML_DIR}/include")

# GLM
target_link_libraries(${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")

# spdlog
target_link_libraries(${PROJECT_NAME} "spdlog" "${SPDLOG_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${SPDLOG_DIR}/include")

# PhysX
target_link_libraries(${PROJECT_NAME} "PhysXExtensions" )
target_link_libraries(${PROJECT_NAME} "PhysX" )
target_link_libraries(${PROJECT_NAME} "PhysXPvdSDK" )
target_link_libraries(${PROJECT_NAME} "PhysXCharacterKinematic")
target_link_libraries(${PROJECT_NAME} "PhysXCooking")
target_link_libraries(${PROJECT_NAME} "PhysXCommon")
target_link_libraries(${PROJECT_NAME} "PhysXFoundation")
target_include_directories(${PROJECT_NAME} PRIVATE "${PHYSX_ROOT_DIR}/include")


# Add physx libraries to target
#target_link_libraries(game_engine PUBLIC ${PHYSX_LIBRARIES})
#### Windows only: Copy the Physx dll files to the simulation executable####

if (TARGET_BUILD_PLATFORM STREQUAL "windows")
	# References NvidiaBuildOptions.cmake to figure out if system is 32/64 bit
	IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET(LIBPATH_SUFFIX "64")
	ELSE()
		SET(LIBPATH_SUFFIX "32")
	ENDIF()
	GetPlatformBinName(PLATFORM_BIN_NAME ${LIBPATH_SUFFIX})
	set(PhysxOutputPath ${PX_OUTPUT_LIB_DIR}/bin/${PLATFORM_BIN_NAME}/)
	message("Physx Output Path: " ${PhysxOutputPath})

	# copy PhysX dll's to build dir. Happens on every build.
	#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${PhysxOutputPath}" "$<TARGET_FILE_DIR:target_name >/..")
    #target_include_directories(${PROJECT_NAME} PRIVATE "${PhysxOutputPath}/debug")
endif()






