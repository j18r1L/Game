#type vertex
#version 410 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

out vec3 v_FragPosition;

void main()
{
	v_FragPosition = a_Position;
    gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0);
}





#type fragment
#version 410 core

layout (location = 0) out vec4 o_Color;

in vec3 v_FragPosition;

float grid(vec2 coords, float res)
{
	vec2 grid = fract(coords * res);
	return (step(res, grid.x) * step(res, grid.y));
}

void main()
{
	float alpha = 1.0 - grid((v_FragPosition.yz + 0.501) * 800., 0.025);
    o_Color = vec4(vec3(0.), alpha);
}
