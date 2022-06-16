#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;       
};

#define NR_DIR_LIGHTS 4
#define NR_POINT_LIGHTS 10
#define NR_SPOT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform int nbDirLight;
uniform int nbPointLight;
uniform int nbSpotLight;
uniform DirLight dirLight[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight[NR_SPOT_LIGHTS];
uniform Material material;

// function prototypes
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 result;
    for(int i = 0; i < nbDirLight; i++)
	result += CalcDirLight(dirLight[i], norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < nbPointLight; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
    for(int i = 0; i < nbSpotLight; i++)
        result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);    
    if(material.diffuse_color != vec3(0,0,0))
        FragColor = vec4(result.x, result.y, result.z, 1.0);
    else
        FragColor = result;
}

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = normalize(lightDir + normalize(viewDir - FragPos));
    float spec = pow(max(dot(normalize(viewDir - FragPos), reflectDir), 0.0), material.shininess);
    // combine results
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    if(material.diffuse_color == vec3(0,0,0))
    {
        ambient = light.ambient * vec4(texture(material.texture_diffuse1, TexCoords));
        diffuse = light.diffuse * diff * vec4(texture(material.texture_diffuse1, TexCoords));
        specular = light.specular * spec * vec4(texture(material.texture_diffuse1, TexCoords));
    }
    else
    {
        ambient = light.ambient * vec4(material.diffuse_color, 1.0);
        diffuse = light.diffuse * diff * vec4(material.diffuse_color, 1.0);
        specular = light.specular * spec * vec4(material.specular_color, 1.0);
    }
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = normalize(lightDir + normalize(viewDir - fragPos));
    float spec = pow(max(dot(normalize(viewDir - fragPos), reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    if(material.diffuse_color == vec3(0,0,0))
    {
        ambient = light.ambient * vec4(texture(material.texture_diffuse1, TexCoords));
        diffuse = light.diffuse * diff * vec4(texture(material.texture_diffuse1, TexCoords));
        specular = light.specular * spec * vec4(texture(material.texture_diffuse1, TexCoords));
    }
    else
    {
        ambient = light.ambient * vec4(material.diffuse_color, 1.0);
        diffuse = light.diffuse * diff * vec4(material.diffuse_color, 1.0);
        specular = light.specular * spec * vec4(material.specular_color, 1.0);
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    if(material.diffuse_color == vec3(0,0,0))
    {
        ambient = light.ambient * vec4(texture(material.texture_diffuse1, TexCoords));
        diffuse = light.diffuse * diff * vec4(texture(material.texture_diffuse1, TexCoords));
        specular = light.specular * spec * vec4(texture(material.texture_diffuse1, TexCoords));
    }
    else
    {
        ambient = light.ambient * vec4(material.diffuse_color, 1.0);
        diffuse = light.diffuse * diff * vec4(material.diffuse_color, 1.0);
        specular = light.specular * spec * vec4(material.specular_color, 1.0);
    }
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}