#pragma once
/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#ifndef _E_CLRD_LINE
#define _E_CLRD_LINE

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "Entity.h"

class E_colored_line : public Entity
{
	ShaderBuffer_POS_COL line_data;
	glm::mat4 modelMat;

public:
	E_colored_line() {}

	void initEntity()
	{
		std::vector<float> v1 =
		{
			0,0,0,
			0,0,0
		};
		std::vector<float> v2 =
		{
			1, 1, 1,
			1, 1, 1	
		};

		line_data = ShaderBuffer_POS_COL(v1, v2);
	}

	void initEntity(std::vector<float> position,
		std::vector<float> color)
	{
		line_data = ShaderBuffer_POS_COL(position, color);
	}

	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glUseProgram(shader->getShaderProgramHandle());
		modelMat = data.glm_model;
		modelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		modelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(6));
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawArrays(GL_LINES, 0, 2);
	}

	~E_colored_line()
	{
		line_data.deleteResource();
	}
	GLuint getVAOHandle() { return line_data.getVAOHandle(); }

};

#endif