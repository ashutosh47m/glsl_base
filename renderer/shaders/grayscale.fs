#version 450 core

in vec2 fs_uv;

uniform sampler2D u_RT1_tex;

void main()
{
	vec4 color = texture( u_RT1_tex, fs_uv );
	float intensity = dot(color.rgb, vec3(.29,.59,.12));
	gl_FragColor = vec4(intensity, intensity, intensity, 1);
	//gl_FragColor = color ;
}