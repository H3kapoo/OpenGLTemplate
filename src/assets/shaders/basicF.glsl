#version 330 core

out vec4 fragColor;

in vec4 colorOut;

void main()
{
   fragColor = vec4(colorOut);
}
