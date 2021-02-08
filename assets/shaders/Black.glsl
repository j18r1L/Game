#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Transform;

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
	vs_Output.WorldPosition = vec3(u_Transform * vec4(a_Position, 1.0));
    vs_Output.Normal = mat3(u_Transform) * a_Normal;
	vs_Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	vs_Output.WorldNormals = mat3(u_Transform) * mat3(a_Tangent, a_Binormal, a_Normal);
	vs_Output.WorldTransform = mat3(u_Transform);
	vs_Output.Binormal = a_Binormal;
	vs_Output.ViewPosition = vec3(u_ViewMatrix * vec4(vs_Output.WorldPosition, 1.0));
	
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 430 core

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

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 o_BloomColor;

uniform vec3 u_CameraPosition;
uniform sampler2D u_AlbedoColor;

void main()
{
	color = texture(u_AlbedoColor, vs_Input.TexCoord);
	//color = vec4(0.0, 0.0, 0.0, 1.0);
}
