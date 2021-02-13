#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Binormal;
layout(location = 4) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Transform;
uniform int u_EntityID;

flat out int v_EntityID;

void main()
{
    v_EntityID = u_EntityID;
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}





#type fragment
#version 410 core

layout (location = 0) out vec4 o_IDBuffer;

flat in int v_EntityID;

void main()
{
    float id = v_EntityID;
    id = id / 255.0;
    o_IDBuffer = vec4(id, id, id, 1.);
    //o_IDBuffer= 200;
    //o_IDBuffer = v_EntityID;
}
