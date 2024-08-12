#version 450

layout(binding = 0) uniform SceneData {
    mat4 view;
    mat4 proj;
} scene_data;

layout(binding = 1) uniform ModelData {
    mat4 mat;
} model_data;

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = scene_data.proj * scene_data.view * vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
    fragColor = vec3(1.0f, 0.0f, 0.0f);
}