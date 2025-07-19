#pragma once

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "Model.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace VoxelEngine
{
    class Renderer
    {
        public:
        Renderer(Window& window, Device& device);
        ~Renderer();

        Renderer(const Renderer &) = delete;
        Renderer& operator=(const Renderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
        float getAspectRatio() const { return swapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame is not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window& window;
        Device& device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}