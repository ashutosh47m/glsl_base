#version 400
// ------------------------ model
flat in vec3 vs_model_v3Position; 
flat in vec3 vs_model_v3Normal;
flat in vec3 vs_model_v3Tangent;
flat in vec3 vs_model_v3Color;
flat in vec2 vs_model_v2UV;
uniform sampler2D texture0;

void main()
{
	vec4 diff = texture( texture0, vs_model_v2UV);
	if(diff.a <=.72f)
 		discard;
	gl_FragData[0] = diff;
	gl_FragData[1] = vec4(0,0,0, 1);
	gl_FragData[2] = vec4(vs_model_v3Position,1);
	gl_FragData[3] = vec4(1,0,1,1);
}
