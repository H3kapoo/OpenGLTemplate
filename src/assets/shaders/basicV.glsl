#version 330 core

layout (location = 0) in vec3 vPos;

uniform vec4 color;
// uniform mat4 model;
// uniform mat4 proj;

out vec4 colorOut;

void main()
{
    colorOut = color;
    // gl_Position = proj * model * vec4(vPos.xyz, 1.0);
    gl_Position = vec4(vPos.xyz, 1.0);
}
