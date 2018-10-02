#version 450 core

in vec3 vs_model_v3Color;
in vec3 fs_position;

void main()
{
	gl_FragData[0] = vec4(1,1,1,1);			// sun
	gl_FragData[1] = vec4(vs_model_v3Color,1); 	// albedo
	gl_FragData[2] = vec4(fs_position,1);		// position
}