#include "App.hpp"

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

    App::App() { loadObjects(); }

    App::~App() {}

    void App::run()
    {
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
            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 10.f);

            if (auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, objects, camera);
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        vkDeviceWaitIdle(device.device());
    }

    void App::loadObjects()
    {
        std::shared_ptr<Model> cubeModel = Model::createModelFromFile(device, "..\\Resources\\Models\\smooth_vase.obj");

        auto cube = Object::createObject();
        cube.model = cubeModel;
        cube.transform.translation = {.0f, .0f, 2.5f};
        cube.transform.scale = {.5f, .5f, .5f};

        objects.push_back(std::move(cube));
    }
}