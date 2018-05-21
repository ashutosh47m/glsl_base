// MOVANIA - OpenGL Development CookBook

#version 450 core

layout(location = 0) out vec4 vFragColor;	//fragment shader output

smooth in vec3 vUV;				//3D texture coordinates form vertex shader 
								//interpolated by rasterizer

//uniform
uniform sampler3D u_volume;		//volume dataset

void main()
{
	//Here we sample the volume dataset using the 3D texture coordinates from the vertex shader.
	//Note that since at the time of texture creation, we gave the internal format as GL_RED
	//we can get the sample value from the texture using the red channel. Here, we set all 4
	//components as the sample value in the texture which gives us a shader of grey.
	gl_FragData[0] = texture(u_volume, vUV).rrrr;
	gl_FragData[1] = vec4(0,0,0,1);
	gl_FragData[2] = vec4(1,1,0,1);
	gl_FragData[3] = vec4(1,0,1,1);
}