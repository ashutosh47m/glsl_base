#version 450 core

in vec2 fs_uv;

uniform sampler2D u_RT1_tex;

void main()
{
	gl_FragColor = texture( u_RT1_tex, fs_uv );
}