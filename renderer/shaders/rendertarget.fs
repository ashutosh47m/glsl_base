#version 450 core

in vec2 fs_uv;

uniform sampler2D u_var_tex;

out vec4 color; 

void main()
{
	color = texture( u_var_tex, fs_uv );
}