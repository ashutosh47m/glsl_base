/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/GL.h>
#include "../resources/shader_program.h"

class glsl_data
{
	// bad style of naming a class, needs to be changed...
public:
	glm::mat4 glm_model; 
	glm::mat4 glm_view ;
	glm::mat4 glm_projection;
	glm::mat4 glm_modelView;	

	glm::vec3 f_position;		// global camera position
								// all the objects in the scene will be 
};

class AbstractScene
{
	ShaderLibrary *shaderlib;
public:
	AbstractScene()
	{ }
	void loadShaders()
	{
		shaderlib = new ShaderLibrary();
		shaderlib->loadShaders();
	}
	~AbstractScene()
	{
		delete shaderlib;
	}

	inline ShaderLibrary* getShaderLibrary() { return shaderlib; }

	GLuint globalTextureCount;
	virtual void	initialize()=0;
	virtual void	draw() = 0;
	virtual void	update() = 0;

	// every scene has certain things which are common for every shaders and objects used.
	// glsl_data contains such items.
	glsl_data data;
};
//
//class keyHandler
//{
//	void keyprocess(WPARAM wParam, LPARAM lParam)
//	{
//	
//	}
//};
#endif