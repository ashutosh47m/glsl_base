/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef SCENE_HANDLER_H
#define SCENE_HANDLER_H

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/GL.h>
#include "../resources/shader_program.h"

class glsl_data
{
	// bad style of naming a class, needs to be changed...
public:
	glsl_data()
	{
		// set the camera here.
		// since the camera is a part of the scene we will be initializing it here.
		glm_view = glm::lookAt(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

		// the global camera matrix, represents where the camera is in the scene 
		glm_model = glm::mat4(1.0f);

		f_position = glm::vec3(0.0f, 0.0f, 5.0f);
	}
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