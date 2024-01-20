#version 450

layout(location = 0) in vec4 inPosition;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = inPosition;
    fragColor = vec3(inPosition.z, 0.0f, 0.0f);
}