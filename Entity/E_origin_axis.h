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
	ShaderBuffer   *m_LineData;
	GLuint			m_VaoHandle;
	glm::mat4		m_ModelMat;

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

		m_LineData = new ShaderBuffer_POS_COL(v1, v2);
		m_VaoHandle = m_LineData->getVAOHandle();
	}

	void initEntity(std::vector<float> position,
		std::vector<float> color)
	{
		m_LineData = new ShaderBuffer_POS_COL(position, color);
		m_VaoHandle = m_LineData->getVAOHandle();
	}

	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glBindVertexArray(m_VaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
		glDrawArrays(GL_LINES, 0, 2);
	}

	~E_colored_line()
	{
		m_LineData->deleteResource();
		delete m_LineData;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }
	
	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
};

#endif