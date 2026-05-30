#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos; // position in world coordinates of the fragment

void main()
{
    float specularStrength = 0.5;
    float ambientStrength = 0.1;

    // ---- diffuse ----
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // direction of our ray of light to the fragment
    float diff = max(dot(norm, lightDir), 0.0); // the diffuse scalar
    vec3 diffuse = diff * lightColor;

    // ---- ambient ----
    vec3 ambient = ambientStrength * lightColor;

    // ---- specular ----
    int shininess = 32;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // -- final output --
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}