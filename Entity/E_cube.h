/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#ifndef _E_CUBE
#define _E_CUBE

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "Entity.h"

class E_cube : public Entity
{
	//ShaderBuffer_POS_IND cube_data;
	GLuint vaoHandle;
	glm::mat4 modelMat;

public: 
	E_cube(){}

	void initEntity()
	{
		std::vector<float> v1 =
		{
			-0.5f,  -0.5f,  -0.5f,
			0.5f,  -0.5f,  -0.5f,
			0.5f,   0.5f,  -0.5f,
			-0.5f,   0.5f,  -0.5f,
			-0.5f,  -0.5f,   0.5f,
			0.5f,  -0.5f,   0.5f,
			0.5f,   0.5f,   0.5f,
			-0.5f,   0.5f,   0.5f
		};

		std::vector<GLushort> v2 =
		{
			0,5,4,
			5,0,1,
			3,7,6,
			3,6,2,
			7,4,6,
			6,4,5,
			2,1,3,
			3,1,0,
			3,0,7,
			7,0,4,
			6,5,2,
			2,5,1
		};

		std::vector<float> v3 =
		{
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f,
			0.5f,  0.5f,   0.5f
		};

		ShaderBuffer *cube_data = new ShaderBuffer_POS_IND(v1, v2);
		vaoHandle = cube_data->getVAOHandle();
	}

	void initEntity(std::vector<float> position,
		std::vector<GLushort> index)
	{
		ShaderBuffer *cube_data = new ShaderBuffer_POS_IND(position, index);
		vaoHandle = cube_data->getVAOHandle();
	}

	// this will draw at origin
	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glBindVertexArray(vaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		modelMat = data.glm_model;
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}
	
	// this will draw at specified position
	void draw(glsl_data& data, ShaderProgram *& shader, glm::vec3 position)
	{
		glBindVertexArray(vaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		modelMat = data.glm_model;
		modelMat *= glm::translate(glm::mat4(1.0f), position);
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}
	
	void enable()
	{
		glBindVertexArray(vaoHandle);
	}
};

#endif