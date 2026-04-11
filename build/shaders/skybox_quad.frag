#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform samplerCube skybox;
uniform mat4 invProjection;
uniform mat4 invView;
void main() {
    vec4 clip = vec4(TexCoords, 1.0, 1.0);
    vec4 worldDir = invProjection * clip;
    worldDir = invView * vec4(worldDir.xyz, 0.0);
    vec3 dir = normalize(worldDir.xyz);
    FragColor = texture(skybox, dir);
}