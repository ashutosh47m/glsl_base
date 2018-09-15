#version 400
layout (location = 0) in vec3 in_model_v3Position;
layout (location = 1) in vec3 in_model_v3Normal;
layout (location = 2) in vec3 in_model_v3Tangent;
layout (location = 3) in vec3 in_model_v3Color;
layout (location = 4) in vec2 in_model_v2UV;

out vec3 vs_model_v3Position; 
out vec3 vs_model_v3Normal;
out vec3 vs_model_v3Tangent;
out vec3 vs_model_v3Color;
out vec2 vs_model_v2UV;

uniform mat4 u_m4MVP;
uniform mat3 u_m3NormalMatrix;

void main()
{
	vs_model_v2UV		= in_model_v2UV;
	vs_model_v3Normal	= normalize(u_m3NormalMatrix * in_model_v3Normal);
	vs_model_v3Tangent	= u_m3NormalMatrix * in_model_v3Tangent;	
	vs_model_v3Color	= in_model_v3Color;
	vs_model_v3Position	= in_model_v3Position;
	gl_Position	  		= u_m4MVP * vec4(in_model_v3Position,1.0);	
}