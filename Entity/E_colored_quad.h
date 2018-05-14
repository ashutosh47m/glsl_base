/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#ifndef _E_CLRD_Q
#define _E_CLRD_Q

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "Entity.h"

class E_colored_quad : public Entity
{
	ShaderBuffer   *m_QuadData = NULL;
	GLuint			m_VaoHandle;
	glm::mat4		m_ModelMat;

public:
	E_colored_quad() {}

	void initEntity() 
	{
		std::vector<float> v1 = 
		{ 
			 .25, -.25, .50,
			-.25, -.25, .50,
			 .25,  .25, .50,
			-.25,  .25, .50,
		};
		std::vector<float> v2 =
		{
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,
			1, 0, 1,
		};

		m_QuadData = new ShaderBuffer_POS_COL(v1, v2);
		m_VaoHandle = m_QuadData->getVAOHandle();
	}

	void draw(glsl_data& data, ShaderProgram *& shader) 
	{
		glBindVertexArray(m_VaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0));
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~E_colored_quad()
	{
		m_QuadData->deleteResource();
		delete m_QuadData;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }
	
	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
};

#endif