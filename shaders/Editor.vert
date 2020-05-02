#version 410 core
layout (location = 0) in vec3 in_Pos;

uniform mat4 model_view_projection;

void main()
{
    gl_Position = model_view_projection * vec4(in_Pos, 1.0);
}
