#version 450 core

in vec3 fs_position;
void main()
{
	gl_FragData[0] = vec4(1,0,0,1);
	gl_FragData[1] = vec4(0,0,0, 1.0); 
}