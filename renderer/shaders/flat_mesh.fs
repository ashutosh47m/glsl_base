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
	vec4 diff = texture( texture0, vs_model_v2UV);
	if(diff.a <=.72f)
 		discard;
	//gl_FragData[0] used for sun
	gl_FragData[1] = diff;						// albedo 
	gl_FragData[2] = vec4(vs_model_v3Position,1);			// position
	gl_FragData[3] = vec4(normalize(vs_model_v3Normal), 1);		// normal
}
