#version 450 core

in vec2 fs_uv;

uniform sampler2D u_RT1_tex;

void main()
{
	// the original
	gl_FragColor = texture( u_RT1_tex, fs_uv );
	
	//1st sept debugging black screen
	//gl_FragColor = vec4(1,0,0,1);
}