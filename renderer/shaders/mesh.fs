#version 400
// ------------------------ model
in vec3 vs_model_v3Position; 
in vec3 vs_model_v3Normal;
in vec3 vs_model_v3Tangent;
in vec3 vs_model_v3Color;
in vec2 vs_model_v2UV;
uniform sampler2D texture0;

void main()
{
	gl_FragData[0] = texture( texture0, vs_model_v2UV);
	gl_FragData[1] = vec4(0,0,0, 1);
	gl_FragData[2] = vec4(vs_model_v3Position,1);
	gl_FragData[3] = vec4(1,0,1,1);
}