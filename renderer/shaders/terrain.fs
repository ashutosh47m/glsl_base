#version 400
// ------------------------ model
flat in vec3 vs_model_v3Position; 
in vec3 vs_model_v3Normal;
flat in vec3 vs_model_v3Tangent;
 in vec2 vs_model_v2UV;

uniform sampler2D u_dryTex;

void main()
{
	vec4 diff = texture( u_dryTex, vs_model_v2UV * 40);
	gl_FragData[1] = diff;
	gl_FragData[2] = vec4(vs_model_v3Position,1);
	gl_FragData[3] = vec4(normalize(vs_model_v3Normal), 1);
}