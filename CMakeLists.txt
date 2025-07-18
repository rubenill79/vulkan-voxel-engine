cmake_minimum_required(VERSION 3.26)

project(vulkan-voxel-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "")
set(GLFW_INSTALL        OFF CACHE BOOL "")

set(ENABLE_GLSLANG_BINARIES  OFF  CACHE BOOL "")
set(ENABLE_HLSL              OFF  CACHE BOOL "")
set(ENABLE_CTEST             OFF  CACHE BOOL "")
set(ENABLE_OPT               OFF  CACHE BOOL "")

set(ASSIMP_NO_EXPORT                      ON CACHE BOOL "")
set(ASSIMP_BUILD_DRACO                    OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS             OFF CACHE BOOL "")
set(ASSIMP_BUILD_TESTS                    OFF CACHE BOOL "")
set(ASSIMP_INSTALL_PDB                    OFF CACHE BOOL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")

include_directories(External/Source/glfw/include)
include_directories(External/Source/vulkan/include)
include_directories(External/Source/glslang)
include_directories(External/Source/glm)
include_directories(External/Source/stb)    
include_directories(External/Source/assimp/include)
include_directories(Shared/Source)

add_subdirectory(External/Source/glfw)
add_subdirectory(External/Source/assimp)
add_subdirectory(External/Source/glslang)
add_subdirectory(Shared)

include(CMake/ExecutableProject.cmake)
add_subdirectory(Game)