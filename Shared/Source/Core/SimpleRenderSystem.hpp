#pragma once

#include "Platform/Device.hpp"
#include "Platform/Pipeline.hpp"
#include "FrameInfo.hpp"
#include "Camera.hpp"
#include "Object.hpp"

#include <memory>
#include <vector>

namespace VoxelEngine
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(Device &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo, std::vector<Object> &objects);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device &device;

        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
    };
}