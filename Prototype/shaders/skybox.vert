#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;


layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragPosition;

void main() {
	fragPosition = inPosition;
	fragPosition.xy *= -1.0;
	gl_Position = ubo.proj * vec4(fragPosition, 1.0);
}