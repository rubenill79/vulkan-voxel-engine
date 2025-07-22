#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUniformBuffer {
    mat4 projectionViewMatrix;
    vec3 directionToLight;
} uniformBuffer;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

const float AMBIENT = 0.02;

void main()
{
    gl_Position = uniformBuffer.projectionViewMatrix * push.modelMatrix * vec4(position, 1.0);

    // temporally: this is only correct in certain situations
    // only works correctly if the scale is uniform (scale.x == scale.y == scale.z)
    // vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);

    // calculating the inverse in a shader can be expensive, so we use a normal matrix
    // vec3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
    // vec3 normalWorldSpace = normalize(normalMatrix * normal);

    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

    float lightIntensity = AMBIENT + max(dot(normalWorldSpace, uniformBuffer.directionToLight), 0);

    fragColor = lightIntensity * color;
}