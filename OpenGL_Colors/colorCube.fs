#version 330 core
out vec4 FragColor;
  
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material 
{
    sampler2D diffuse;
    sampler2D specular; 
    sampler2D emission;
    float shininess;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;  
uniform float time;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos; // position in world coordinates of the fragment

void main()
{

    // ---- diffuse ----
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); // direction of our ray of light to the fragment
    float diff = max(dot(norm, lightDir), 0.0); // the diffuse scalar
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    // ---- ambient ----
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // ---- specular ----
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    // ---- emission ----
    vec3 emission = vec3(0.0);

    // only draws if within the y bounds of the wooden area
    float margin = 0.082;
    float mask = 1.0;
    if ( TexCoords.y < margin ) mask *= 0.0; 
    if ( TexCoords.y > 1.0-margin ) mask *= 0.0;

    if (mask > 0.0)
    {
        for (int i = 0; i < 5; i++)
        {
            vec2 emissionCoords = TexCoords - vec2(0.01, time*i*0.2);
            emission += vec3(texture(material.emission, emissionCoords))*i*0.2;
        }
    }

    // -- final output --
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);

}