#version 450 core

layout (location = 0) in vec3 in_model_v3Position;
layout (location = 3) in vec3 in_model_v3Color;
layout (location = 4) in vec2 in_model_v2UV;

out vec3 vs_model_v3Color;
out vec3 fs_position;

uniform mat4 u_m4MVP;

void main()
{
	vs_model_v3Color = in_model_v3Color;
	gl_Position = u_m4MVP * vec4(in_model_v3Position, 1);
	fs_position = gl_Position.xyz;
}