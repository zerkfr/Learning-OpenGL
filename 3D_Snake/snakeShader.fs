#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform vec3[] pos;

void main()
{

    //FragColor = vec4(pos[0], 1.0f);
    FragColor = vec4(color, 1.0f);
} 