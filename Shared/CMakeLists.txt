cmake_minimum_required(VERSION 3.26)

project(VoxelEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "Source/*.cpp")
file(GLOB_RECURSE HEADERS "Source/*.h")

add_library(VoxelEngine STATIC ${SOURCES} ${HEADERS})

#target_precompile_headers(VoxelEngine 
#    PRIVATE 
#        "Source/pch.h"
#)


execute_process(
    COMMAND ./shaders.bat
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VoxelEngine 
    PRIVATE 
        glfw
        glslang
        "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
        assimp
)

set(RESOURCE_LIMITS_DIR ${CMAKE_SOURCE_DIR}/External/Source/glslang/glslang/ResourceLimits)

target_sources(VoxelEngine PRIVATE 
    ${RESOURCE_LIMITS_DIR}/ResourceLimits.cpp
    ${RESOURCE_LIMITS_DIR}/resource_limits_c.cpp)