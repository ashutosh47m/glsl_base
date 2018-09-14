#version 450 core

in vec3 fs_color;
in vec3 fs_position;

void main()
{
	gl_FragData[0] = vec4(fs_color,1);
	gl_FragData[1] = vec4(1,1,1,1);		// making the cube as sun in ter_scene ;) 
	gl_FragData[2] = vec4(fs_position,1);
	gl_FragData[3] = vec4(1,0,1,1);
}