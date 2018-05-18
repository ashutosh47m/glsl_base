#version 450 core

in vec3 fs_color;
in vec3 fs_position;
in vec2 fs_uv;

uniform sampler2D u_var_tex;

void main()
{
	gl_FragData[0] = texture( u_var_tex, fs_uv );
	gl_FragData[1] = vec4(fs_position,1);
	gl_FragData[2] = vec4(1,1,0,1);
	gl_FragData[3] = vec4(1,0,1,1);

	// add color to the texture if you want,
	//color +=vec4(fs_color, 1.0f);
}