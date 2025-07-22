#include "App.hpp"

#include "Platform/Buffer.hpp"
#include "Platform/Texture.hpp"

#include "Camera.hpp"
#include "KeyboardController.hpp"
#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp> // Para glm::radians

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

    App::App()
    {
        globalPool = DescriptorPool::Builder(device)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

        loadObjects();
    }

    App::~App() {}

    void App::run()
    {
        std::vector<std::unique_ptr<Buffer>> uniformBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uniformBuffers.size(); i++)
        {
            uniformBuffers[i] = std::make_unique<Buffer>(
                device,
                sizeof(GlobalUniformBuffer),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uniformBuffers[i]->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
            .build();

        Texture texture = Texture(device, "..\\Resources\\Textures\\qilin.jpg");

        VkDescriptorImageInfo imageInfo{};
        imageInfo.sampler = texture.getSampler();
        imageInfo.imageView = texture.getImageView();
        imageInfo.imageLayout = texture.getImageLayout();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uniformBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .writeImage(1, &imageInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
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

            // Para simular la luz del sol que se mueve
            static float totalTime = 0.f;
            totalTime += frameTime;

            // Ángulo que avanza con el tiempo (velocidad ajustable)
            float lightAngle = totalTime * glm::radians(10.f); // gira 10 grados por segundo

            // Calculamos la dirección de la luz como un vector que rota sobre el eje X (puedes elegir otro eje)
            glm::vec3 lightDir = glm::normalize(glm::vec3{
                cos(lightAngle),
                -sin(lightAngle),
                -1.f});

            // Supongamos que quieres que rote a 30 grados por segundo
            float rotationSpeedDegrees = 30.f;
            float rotationSpeedRadians = glm::radians(rotationSpeedDegrees);

            // Asumiendo que el objeto que quieres girar es el primero (objeto 0)
            if (!objects.empty())
            {
                float newYRotation = objects[0].transform.rotation.y + rotationSpeedRadians * frameTime;
            
                // Mantener el ángulo en [0, 2pi]
                if (newYRotation > glm::two_pi<float>()) {
                    newYRotation -= glm::two_pi<float>();
                }
            
                objects[0].transform.rotation.y = newYRotation;
            }

            cameraController.moveInPlaneXZ(window.getGLFWWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspectRatio = renderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspectRatio, aspectRatio, -1.f, 1.f, -1.f, 1.f);
            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 100.f);

            if (auto commandBuffer = renderer.beginFrame())
            {
                int frameIndex = renderer.getFrameIndex();
                FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

                // Update global uniform buffer
                GlobalUniformBuffer ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                ubo.lightDirection = lightDir; // <-- actualizamos la luz aquí

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
        std::shared_ptr<Model> smoothVaseModel = Model::createModelFromFile(device, "..\\Resources\\Models\\qilin.obj");
        std::shared_ptr<Model> flatVaseModel = Model::createModelFromFile(device, "..\\Resources\\Models\\flat_vase.obj");

        auto obj1 = Object::createObject();
        obj1.model = smoothVaseModel;
        obj1.transform.translation = {-.5f, .5f, 2.5f};
        float radianes = glm::radians(180.0f);
        obj1.transform.rotation = {0.f, 0.f, 0.f};
        // obj1.transform.scale = {3.f, 1.5f, 3.f};
        obj1.transform.scale = {.001f, .001f, .001f};

        objects.push_back(std::move(obj1));

        // auto obj2 = Object::createObject();
        // obj2.model = flatVaseModel;
        // obj2.transform.translation = {.5f, .5f, 2.5f};
        // obj2.transform.scale = {3.f, 1.5f, 3.f};
        //
        // objects.push_back(std::move(obj2));
    }
}