#version 450 core

layout (location = 0) in vec3 in_vs_position;
layout (location = 1) in vec3 in_vs_color;
layout (location = 2) in vec2 in_vs_uv;

out vec3 fs_color;
out vec3 fs_position;
out vec2 fs_uv;
uniform mat4 u_m4MVP;

void main()
{
	fs_uv = in_vs_uv ;
	fs_color = in_vs_color;
	gl_Position =u_m4MVP * vec4(in_vs_position, 1);
	fs_position = gl_Position.xyz;
}