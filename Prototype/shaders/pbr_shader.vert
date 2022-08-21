#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitTangent;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragColor;
layout(location = 2) out vec2 fragTexCoord;
layout(location = 3) out vec3 fragNormal;

layout(location = 4) out vec3 tangentLightPos;
layout(location = 5) out vec3 tangentViewPos;
layout(location = 6) out vec3 tangentFragPos;


void main() {
	fragPosition = vec3(ubo.model * vec4(inPosition, 1.0));
    fragColor = inColor;
    fragTexCoord = inTexCoord;
	fragNormal = mat3(ubo.model) * inNormal;

    mat3 normalMatrix = transpose(inverse(mat3(ubo.model)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    tangentLightPos = TBN * vec3(0.0, 1.5, 0.5);
    tangentViewPos = TBN * vec3(0.0,0.0,0.0);
    tangentFragPos = TBN * fragPosition;

	gl_Position = ubo.proj * ubo.view * vec4(fragPosition, 1.0);
}