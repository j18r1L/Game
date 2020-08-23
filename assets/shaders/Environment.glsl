#type vertex
#version 430 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionView;

out vec3 v_FragPos;

void main()
{
	v_FragPos = a_Position;
    gl_Position = u_ProjectionView * vec4(a_Position, 1.0);
}



#type fragment
#version 430 core

layout (location = 0) out vec4 o_Color;

in vec3 v_FragPos;

void main()
{
	
    vec3 direction = mix(normalize(v_FragPos), vec3(1.), 0.8);
    o_Color = vec4(direction, 1.0);
}
