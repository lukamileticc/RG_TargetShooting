#version 330 core

out vec4 FragColor;

uniform vec4 gColor;

void main()
{
    FragColor = gColor;
}