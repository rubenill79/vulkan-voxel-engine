#pragma once

#include "Camera.hpp"

#include <vulkan/vulkan.h>

namespace VoxelEngine
{

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera &camera;
    };
}