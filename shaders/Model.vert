#version 410 core
layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoords;


uniform mat4 model_view_projection;

out vec2 fs_TexCoords;


void main()
{
    gl_Position = model_view_projection * vec4(in_Pos, 1.0);
    fs_TexCoords = in_TexCoords;
}
