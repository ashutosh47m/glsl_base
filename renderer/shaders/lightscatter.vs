#version 450 core

layout (location = 0) in vec3 in_vs_position;
layout (location = 1) in vec2 in_vs_uv;

out vec2 fs_uv;
uniform mat4 u_m4MVP;

void main()
{
	fs_uv = in_vs_uv ;
	gl_Position =u_m4MVP * vec4(in_vs_position, 1);
}