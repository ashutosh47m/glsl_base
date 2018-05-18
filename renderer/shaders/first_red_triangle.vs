#version 450 core

layout (location = 0) in vec3 position;
uniform mat4 u_m4MVP;

out vec3 fs_position;

void main()
{
	gl_Position = u_m4MVP * vec4(position, 1);
	fs_position = gl_Position.xyz;
}