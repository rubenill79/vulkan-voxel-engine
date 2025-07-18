cmake_minimum_required(VERSION 3.26)

project(Shared)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "Source/*.cpp")
file(GLOB_RECURSE HEADERS "Source/*.h")

add_library(Shared STATIC ${SOURCES} ${HEADERS})

#target_precompile_headers(Shared 
#    PRIVATE 
#        "Source/pch.h"
#)

target_link_libraries(Shared 
    PRIVATE 
        glfw
        glslang
        "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
        assimp
)

set(RESOURCE_LIMITS_DIR ${CMAKE_SOURCE_DIR}/External/Source/glslang/glslang/ResourceLimits)

target_sources(Shared PRIVATE 
    ${RESOURCE_LIMITS_DIR}/ResourceLimits.cpp
    ${RESOURCE_LIMITS_DIR}/resource_limits_c.cpp)