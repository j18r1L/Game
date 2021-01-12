#type vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoords;
    gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}





#type fragment
#version 410 core

layout (location = 0) out vec4 o_Color;

uniform sampler2D u_diffuse;

in vec2 v_TexCoord;

void main()
{
    vec4 color = vec4(1.0);
    //color = texture(u_diffuse, v_TexCoord);
    o_Color = vec4(color);
}
