#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;


struct MeshMatrixStruct
{
	mat4 transform;
};

struct FrameData
{
	vec4 timingData;

	vec4 viewportData;

};

struct CameraMatrixStruct
{
	mat4 view;
	mat4 proj;
	mat4 projView;
	
	mat4 viewInv;
	mat4 projInv;
	mat4 projViewInv;

	mat4 viewPrevFrame;
	mat4 projPrevFrame;
	mat4 projViewPrevFrame;
};

uniform CameraMatrixStruct u_CameraData;

uniform FrameData u_FrameData;

uniform MeshMatrixStruct u_MeshData;

out VertexOutput
{
	vec3 WorldPosition;
    vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;
	vec3 ViewPosition;
} vs_Output;

void main()
{
	vs_Output.WorldPosition = vec3(u_MeshData.transform * vec4(a_Position, 1.0));
    vs_Output.Normal = mat3(u_MeshData.transform) * a_Normal;
	vs_Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	vs_Output.WorldNormals = mat3(u_MeshData.transform) * mat3(a_Tangent, a_Binormal, a_Normal);
	vs_Output.WorldTransform = mat3(u_MeshData.transform);
	vs_Output.Binormal = a_Binormal;
	vs_Output.ViewPosition = vec3(u_CameraData.view * vec4(vs_Output.WorldPosition, 1.0));
	
	gl_Position = u_CameraData.projView * u_MeshData.transform * vec4(a_Position, 1.0);
}

#type fragment
#version 430 core

#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 2
#define MAX_NUM_OF_POINT_LIGHTS 2
#define MAX_NUM_OF_SPOT_LIGHTS 2

in VertexOutput
{
	vec3 WorldPosition;
    vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;
	vec3 ViewPosition;
} vs_Input;

struct s_DirectionalLight 
{
    vec3 l_Direction;
    vec3 l_Color;
    float l_Intensity;
};

struct s_PointLight 
{
	vec3 l_Position;
    vec3 l_Color;
    float l_Intensity;
};

struct s_SpotLight 
{
	vec3 l_Position;
    vec3 l_Direction;
    vec3 l_Color;
    float l_Intensity;
    float l_InnerConeAngle;
    float l_OuterConeAngle;
};

layout(location = 0) out vec4 color;

// Light structs
uniform s_DirectionalLight u_DirectionalLights;
uniform s_PointLight u_PointLights;
uniform s_SpotLight u_SpotLights;

// texture inputs
uniform sampler2D u_AlbedoTexture;

// Toggles
uniform float u_AlbedoTexToggle;

uniform vec3 u_AlbedoColor;

// Calculates the color when using a directional light.
vec3 CalculateDirectionalLight(s_DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.l_Direction);
    // diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);

    // combine results
    vec3 ambient = vec3(0.1);

    return (ambient + diffuse + specular) * light.l_Intensity * light.l_Color;
}

// Calculates the color when using a point light.
vec3 CalculatePointLight(s_PointLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.l_Position - vs_Input.WorldPosition);
    // diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);
	
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);

    // combine results
    vec3 ambient = vec3(0.1);

    return (ambient + diffuse + specular) * light.l_Intensity * light.l_Color;
}

// Calculates the color when using a spot light.
vec3 CalculateSpotLight(s_SpotLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.l_Position - vs_Input.WorldPosition);
    // diffuse shading
    float diffuse = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(viewDirection, reflectDir), 0.0), 32.0);

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.l_Direction)); 
    float epsilon = light.l_InnerConeAngle - light.l_OuterConeAngle;
    float intensity = clamp((theta - light.l_OuterConeAngle) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = vec3(0.1);

    return (ambient + diffuse + specular) * light.l_Intensity * light.l_Color * intensity;
}


void main()
{
	color = texture(u_AlbedoTexture, vs_Input.TexCoord);

    vec3 viewDirection = normalize(vs_Input.ViewPosition - vs_Input.WorldPosition);
    vec3 normal = normalize(vs_Input.Normal);

	// Standard inputs
	vec3 Albedo = u_AlbedoTexToggle > 0.5 ? texture(u_AlbedoTexture, vs_Input.TexCoord).rgb : u_AlbedoColor; 

    vec3 light = vec3(0.0);
    // Directional lighting
    //for(int i = 0; i < 2; i++)
        light += CalculateDirectionalLight(u_DirectionalLights, normal, viewDirection);

    // Point lights
    //for(int i = 0; i < 2; i++)
        light += CalculatePointLight(u_PointLights, normal, viewDirection);
        
    // Spot light
    //for(int i = 0; i < 2; i++)
        light += CalculateSpotLight(u_SpotLights, normal, viewDirection);


	color = vec4(Albedo * light, 1.0);
}