#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform bool dead;
uniform vec3 deadColor;
uniform vec3[] pos;

void main()
{

    //FragColor = vec4(pos[0], 1.0f);
    if (dead)
        FragColor = vec4(deadColor, 1.0f);

    else
        FragColor = vec4(color, 1.0f);
} 