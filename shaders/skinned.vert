#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIndices;
layout (location = 4) in vec4 aBoneWeights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneMatrices[100];   // 最多100根骨骼

void main() {
    mat4 boneTransform = 
        aBoneWeights.x * boneMatrices[aBoneIndices.x] +
        aBoneWeights.y * boneMatrices[aBoneIndices.y] +
        aBoneWeights.z * boneMatrices[aBoneIndices.z] +
        aBoneWeights.w * boneMatrices[aBoneIndices.w];
    vec4 localPos = boneTransform * vec4(aPos, 1.0);
    vec4 worldPos = model * localPos;
    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(model))) * (mat3(boneTransform) * aNormal);
    TexCoords = aTexCoords;
    gl_Position = projection * view * worldPos;
}