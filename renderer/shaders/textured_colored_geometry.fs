#version 450 core

in vec3 fs_color;
in vec2 fs_uv;

uniform sampler2D u_var_tex;

out vec4 color; 

void main()
{
	color = texture( u_var_tex, fs_uv );
	color +=vec4(fs_color, 1.0f);
	
	//color = vec4(fs_color,1);
}