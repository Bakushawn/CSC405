#version 330 core

in vec2 TexCoord;
in vec4 vertexPos;


out vec4 FragColor;

uniform vec4 boxColor;
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
    // FragColor = vec4(vertexPos.x + 0.5, vertexPos.y + 0.5, vertexPos.z + 0.5, 1.0);
}