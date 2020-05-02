#version 410 core
out vec4 FragColor;

uniform sampler2D texture_diffuse;

in vec2 fs_TexCoords;

void main()
{
    FragColor = texture(texture_diffuse, fs_TexCoords);
}
