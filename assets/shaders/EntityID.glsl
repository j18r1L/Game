#type vertex
#version 410 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;
uniform int u_EntityID;

flat out int v_EntityID;

void main()
{
    v_EntityID = u_EntityID;
    gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}





#type fragment
#version 410 core

layout (location = 0) out int o_IDBuffer;

flat in int v_EntityID;

void main()
{
    o_IDBuffer = v_EntityID;
}
