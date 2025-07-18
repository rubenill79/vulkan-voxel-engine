#pragma once

#include "Device.hpp"

#include <string>
#include <vector>

namespace VoxelEngine
{
    struct PipelineConfigInfo {
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline
    {
        public: 
        Pipeline(
            Device& device, 
            const std::string& vertexShaderPath, 
            const std::string& fragmentShaderPath, 
            const PipelineConfigInfo& configInfo);
        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

        private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(
            const std::string& vertexShaderPath, 
            const std::string& fragmentShaderPath,
            const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code,VkShaderModule* shaderModule);

        Device& device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertexShaderModule;
        VkShaderModule fragmentShaderModule;
    };
}