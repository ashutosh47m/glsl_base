#version 450 core

in vec3 fs_color;

void main()
{
	gl_FragData[0] = vec4(fs_color,1);;
	gl_FragData[1] = vec4(1,0,0,1);
	gl_FragData[2] = vec4(1,1,0,1);
	gl_FragData[3] = vec4(1,0,1,1);
}