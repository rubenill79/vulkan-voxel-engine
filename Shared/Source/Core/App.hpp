#pragma once

#include "Platform/Window.hpp"
#include "Platform/Device.hpp"
#include "Platform/Renderer.hpp"
#include "Object.hpp"

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
        void loadObjects();

        Window window{ width, height, "Hello World!" };
        Device device{ window };
        Renderer renderer{ window, device };
        
        std::vector<Object> objects;
    };
}