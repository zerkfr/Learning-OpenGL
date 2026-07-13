#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform bool dead;
uniform vec3 deadColor;
uniform vec3[] pos;

uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 normal;
in vec3 fragPos;


void main()
{

    //FragColor = vec4(pos[0], 1.0f);
    if (dead)
        FragColor = vec4(deadColor, 1.0f);

    else
    {
        // ambient 
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;

        // diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos); 
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        vec3 result = (ambient + diffuse) * color;
        FragColor = vec4(result, 1.0);
    }
} 