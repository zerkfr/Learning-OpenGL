#version 330 core
out vec4 FragColor;
  
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    bool flashOn;
};

struct Material 
{
    sampler2D diffuse;
    sampler2D specular; 
    sampler2D emission;
    float shininess;
};

//#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[4];
uniform DirLight dirLight;
uniform SpotLight spotlight;

uniform vec3 viewPos;
uniform Material material;
uniform float time;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos; // position in world coordinates of the fragment

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 norm = normal;
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    return (ambient + diffuse + specular);
}


vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 norm = normal;
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                        light.quadratic * (distance * distance)); 

    ambient *= attenuation;        
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{

    vec3 norm = normal;
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.outerCutOff && light.flashOn == true)
    {
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                            light.quadratic * (distance * distance)); 

        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        // ambient *= attenuation * intensity;        
        diffuse *= attenuation * intensity;
        specular *= attenuation * intensity;

        return (ambient + diffuse + specular);
    }
    else
    {
        return (light.ambient * vec3(texture(material.diffuse, TexCoords)));
    }

}

void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   //vec3 result = calculateDirectionalLight(dirLight, norm, viewDir); // directional 'sun' light
   vec3 result = vec3(0.0);

   for(int i = 0; i < 4; i++)
   {
       result += calculatePointLight(pointLights[i], norm, viewDir, FragPos);
   }

   result += calculateSpotLight(spotlight, norm, viewDir, FragPos);

   FragColor = vec4(result, 1.0);
    
}

