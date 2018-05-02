#version 450 core

layout (location = 0) in vec3 position;
uniform mat4 u_m4MVP;

void main()
{
/*
	vec4 vertices[3] = 
	vec4[3] 
	(
		vec4( .25, -.25, .50, 1),
		vec4(-.25, -.25, .50, 1),
		vec4( .25,  .25, .50, 1)
	);

	gl_Position = vertices [gl_VertexID];
*/

	gl_Position =u_m4MVP * vec4(position, 1);
}