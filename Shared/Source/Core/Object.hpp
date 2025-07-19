#pragma once

#include "Platform/Model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace VoxelEngine
{

    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)

        /*
        glm::mat4 mat4()
        {
            auto transform = glm::translate(glm::mat4(1.f), translation);

            transform = glm::rotate(transform, rotation.y, glm::vec3(0.f, 1.f, 0.f));
            transform = glm::rotate(transform, rotation.x, glm::vec3(1.f, 0.f, 0.f));
            transform = glm::rotate(transform, rotation.z, glm::vec3(0.f, 0.f, 1.f));

            transform = glm::scale(transform, scale);
            return transform;
        }
        */

        // Faster version of mat4() for 3D transformations
        // Matrix corresponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4()
        {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);

            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0f,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0f,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f}};
        }
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