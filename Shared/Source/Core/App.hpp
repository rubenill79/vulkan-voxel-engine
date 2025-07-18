#pragma once

#include "Platform/Window.hpp"
#include "Platform/Pipeline.hpp"
#include "Platform/Device.hpp"
#include "Platform/SwapChain.hpp"
#include "Platform/Model.hpp"

#include <memory>
#include <vector>

namespace VoxelEngine
{
    class App
    {
        public:
        static constexpr int width = 800;
        static constexpr int height = 600;

        App();
        ~App();

        App(const App &) = delete;
        App& operator=(const App &) = delete;

        void run();

        private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);

        void sierpinski(
            std::vector<Model::Vertex> &vertices,
            int depth,
            glm::vec2 left,
            glm::vec2 right,
            glm::vec2 top);

        Window window{ width, height, "Hello World!" };
        Device device{ window };
        std::unique_ptr<SwapChain> swapChain;
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
    };
}