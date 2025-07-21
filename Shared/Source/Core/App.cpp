#include "App.hpp"

#include "Platform/Buffer.hpp"
#include "Camera.hpp"
#include "KeyboardController.hpp"
#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>

namespace VoxelEngine
{

    struct GlobalUniformBuffer
    {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };
    

    App::App() { loadObjects(); }

    App::~App() {}

    void App::run()
    {
        std::vector<std::unique_ptr<Buffer>> uniformBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uniformBuffers.size(); i++) {
          uniformBuffers[i] = std::make_unique<Buffer>(
              device,
              sizeof(GlobalUniformBuffer),
              1,
              VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
          uniformBuffers[i]->map();
        }

        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
        Camera camera{};
        // camera.setViewDirection(glm::vec3{0.f}, glm::vec3{0.5f, 0.f, 1.f});
        camera.setViewTarget(glm::vec3{-1.0f, -2.0f, 2.0f}, glm::vec3{0.f, 0.f, 2.5f});

        auto viewerObject = Object::createObject();
        KeyboardController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window.shouldClose())
        {
            glfwPollEvents();
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            // frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(window.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspectRatio = renderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 100.f);

            if (auto commandBuffer = renderer.beginFrame())
            {
                int frameIndex = renderer.getFrameIndex();
                FrameInfo frameInfo{
                    .frameIndex = frameIndex,
                    .frameTime = frameTime,
                    .commandBuffer = commandBuffer,
                    .camera = camera
                };
                
                // Update global uniform buffer
                GlobalUniformBuffer ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uniformBuffers[frameIndex]->writeToBuffer(&ubo);
                uniformBuffers[frameIndex]->flush();

                // Render
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, objects);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device());
    }

    void App::loadObjects()
    {
        std::shared_ptr<Model> smoothVaseModel = Model::createModelFromFile(device, "..\\Resources\\Models\\smooth_vase.obj");
        std::shared_ptr<Model> flatVaseModel = Model::createModelFromFile(device, "..\\Resources\\Models\\flat_vase.obj");

        auto obj1 = Object::createObject();
        obj1.model = smoothVaseModel;
        obj1.transform.translation = {-.5f, .5f, 2.5f};
        obj1.transform.scale = {3.f, 1.5f, 3.f};
        // obj1.transform.scale = {.001f, .001f, .001f};

        objects.push_back(std::move(obj1));

        auto obj2 = Object::createObject();
        obj2.model = flatVaseModel;
        obj2.transform.translation = {.5f, .5f, 2.5f};
        obj2.transform.scale = {3.f, 1.5f, 3.f};

        objects.push_back(std::move(obj2));
    }
}