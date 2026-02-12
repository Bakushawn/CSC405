#version 330 core

in vec4 vertexPos;

out vec4 FragColor;

uniform vec4 boxColor;

void main()
{
    FragColor = vec4(vertexPos.x + 0.5, vertexPos.y + 0.5, vertexPos.z + 0.5, 1.0);
}