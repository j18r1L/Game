#type vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
    v_TexCoord = a_TexCoords;
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));

    gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}





#type fragment
#version 410 core

layout (location = 0) out vec4 o_Color;

struct s_DirectionalLight 
{
    vec3 l_Direction;
    vec3 l_Color;
    float l_Intensity;
    float l_Range;
};

struct s_PointLight 
{
	vec3 l_Position;
    vec3 l_Color;
    float l_Intensity;
    float l_Range;
};

struct s_SpotLight 
{
	vec3 l_Position;
    vec3 l_Direction;
    vec3 l_Color;
    float l_Intensity;
    float l_Range;  
    float l_InnerConeAngle;
    float l_OuterConeAngle;
};

// Arrays of lights
uniform int u_NumDirectionalLights = 0;
uniform int u_NumPointLights = 0;
uniform int u_NumSpotLights = 0;

#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 8
#define MAX_NUM_OF_POINT_LIGHTS 8
#define MAX_NUM_OF_SPOT_LIGHTS 8

uniform s_DirectionalLight directionalLights[MAX_NUM_OF_DIRECTIONAL_LIGHTS];
uniform s_PointLight pointLights[MAX_NUM_OF_POINT_LIGHTS];
uniform s_SpotLight spotLights[MAX_NUM_OF_SPOT_LIGHTS];

// Textures
uniform sampler2D u_diffuse;
uniform sampler2D u_specular;

// View position
uniform vec3 u_ViewPosition;

// Inputs
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

// Function prototypes
vec3 CalculateDirectionalLight(s_DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(s_PointLight light, vec3 normal, vec3 viewDirection);
vec3 CalculateSpotLight(s_SpotLight light, vec3 normal, vec3 viewDirection);

void main()
{
    vec3 viewDirection = normalize(u_ViewPosition - v_FragPos);
    vec3 normal = normalize(v_Normal);

    vec3 color = vec3(0., 0., 0.);
    // Directional lighting
    for(int i = 0; i < u_NumDirectionalLights; i++)
        color += CalculateDirectionalLight(directionalLights[i], normal, viewDirection);

    // Point lights
    for(int i = 0; i < u_NumPointLights; i++)
        color += CalculatePointLight(pointLights[i], normal, viewDirection);   
        
    // Spot light
    for(int i = 0; i < u_NumSpotLights; i++)
        color += CalculateSpotLight(spotLights[i], normal, viewDirection);    
    
    o_Color = vec4(color, 1.0);
}

// Calculates the color when using a directional light.
vec3 CalculateDirectionalLight(s_DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.l_Direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);

    // combine results
    vec3 ambient = vec3(texture(u_diffuse, v_TexCoord)) * 0.1;
    vec3 diffuse = diff * vec3(texture(u_diffuse, v_TexCoord));
    vec3 specular = spec * vec3(texture(u_specular, v_TexCoord));

    return (ambient + diffuse + specular) * light.l_Intensity * light.l_Color;
}

// Calculates the color when using a point light.
vec3 CalculatePointLight(s_PointLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.l_Position - v_FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
	
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);

    // combine results
    vec3 ambient = vec3(texture(u_diffuse, v_TexCoord)) * 0.1;
    vec3 diffuse = diff * vec3(texture(u_diffuse, v_TexCoord));
	vec3 specular = spec * vec3(texture(u_specular, v_TexCoord));

    return (ambient + diffuse + specular) * light.l_Intensity * light.l_Color;
}

// Calculates the color when using a spot light.
vec3 CalculateSpotLight(s_SpotLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.l_Position - v_FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.l_Direction)); 
    float epsilon = light.l_InnerConeAngle - light.l_OuterConeAngle;
    float intensity = clamp((theta - light.l_OuterConeAngle) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = vec3(texture(u_diffuse, v_TexCoord)) * 0.1;
    vec3 diffuse = diff * vec3(texture(u_diffuse, v_TexCoord));
    vec3 specular = spec * vec3(texture(u_specular, v_TexCoord));

    return (ambient + diffuse + specular) * light.l_Intensity * light.l_Color * intensity;
}
