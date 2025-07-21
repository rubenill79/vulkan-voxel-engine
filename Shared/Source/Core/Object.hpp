#pragma once

#include "Platform/Model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace VoxelEngine
{

    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 rotation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        // uniform scaling
        // float scale = 1.f;

        glm::mat4 mat4();
        glm::mat3 normalMatrix();
    };

    class Object
    {
    public:
        using id_t = unsigned int;

        static Object createObject()
        {
            static id_t nextId = 0;
            return Object(nextId++);
        }

        Object(const Object &) = delete;
        Object &operator=(const Object &) = delete;
        Object(Object &&) = default;
        Object &operator=(Object &&) = default;

        id_t getId() { return id; }

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        TransformComponent transform{};

    private:
        Object(id_t objId) : id{objId} {}
        id_t id;
    };
}