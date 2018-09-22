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
	GLuint			m_VaoHandle;
	glm::mat4		m_ModelMat;
	ShaderBuffer   *m_CubeData = NULL;
public: 
	E_cube(){}

	void initEntity(bool colored)
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
			1.0f,  0.0f,   0.0f,
			1.0f,   .0f,   1.0f,
			0.0f,  0.0f,   1.0f,
			 .0f,  1.0f,   1.0f,
			0.5f,  0.5f,    .0f,
			0.0f,  0.0f,    .0f,
			1.0f,  0.0f,   0.0f,
			0.0f,  1.0f,   0.0f,
		};


		if (colored)
		{
			m_CubeData = new ShaderBuffer_POS_IND_COL(v1, v2, v3);
			m_VaoHandle = m_CubeData->getVAOHandle();
		}
		else
		{
			m_CubeData = new ShaderBuffer_POS_IND(v1, v2);
			m_VaoHandle = m_CubeData->getVAOHandle();
		}
	}

	void initEntity(std::vector<float> position,
		std::vector<GLushort> index)
	{
		m_CubeData = new ShaderBuffer_POS_IND(position, index);
		m_VaoHandle = m_CubeData->getVAOHandle();
	}

	// this will draw at origin
	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glBindVertexArray(m_VaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}
	
	// draw at the given modelmat
	void draw(glsl_data& data, ShaderProgram *& shader, glm::mat4& modelMat)
	{
		glBindVertexArray(m_VaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}

	// this will draw at specified position
	void draw(glsl_data& data, ShaderProgram *& shader, glm::vec3 position)
	{
		// you can also call m_CubeData->getVAOHandle here, but its best to avoid the extra function call in renderloop
		glBindVertexArray(m_VaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::translate(glm::mat4(1.0f), position);
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}

	glm::mat4 getModelMatrix() { return m_ModelMat; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
	~E_cube()
	{
		m_CubeData->deleteResource();
		delete m_CubeData;
	}

};

#endif