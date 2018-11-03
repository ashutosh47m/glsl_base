#version 400
in vec2 vs_model_v2UV;
uniform sampler2D texture0;

void main()
{
	vec4 diff = texture( texture0, vs_model_v2UV);
	if(diff.a <=.72f)
 		discard;
	gl_FragColor = vec4(1,1,1,1);
}
