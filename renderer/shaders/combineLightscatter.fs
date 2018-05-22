#version 450 core

in vec2 fs_uv;

uniform sampler2D u_godraysIntermediateTex;
uniform sampler2D u_colorTex;

void main()
{
	gl_FragColor  = 
	texture2D(u_colorTex,fs_uv) + 
	(texture2D(u_godraysIntermediateTex, fs_uv)
	*vec4(1.200, .449, 0.000,1)
	)	; 
	//	gl_FragColor = texture( u_RT1_tex, fs_uv );
}