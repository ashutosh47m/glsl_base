/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#ifndef _E_TXTRD_Q
#define _E_TXTRD_Q

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "../resources/texture_handler.h"
#include "Entity.h"

class E_textured_quad : public Entity
{
	ShaderBuffer			*m_QuadData=NULL;
	glm::mat4			 	 m_ModelMat;
	texture				 	 m_T;
	GLuint					 m_VaoHandle;

public:
	E_textured_quad() {}
	void initEntity() {}

	void initEntity(GLuint globalTextureCount, std::string textureName) 
	{
		m_T = texture(globalTextureCount, "u_var_tex", textureName);

		std::vector<float> v1 = 
		{ 
			.25, -.25, .50,
			-.25, -.25, .50,
			.25,  .25, .50,
			-.25,  .25, .50,
		};
		std::vector<float> v2 =
		{
			1,	0, 0,
			0,	1, 0,
			0,	0, 1,
			1,	0, 1,
		};

		std::vector<float> v3 =
		{
			0,	0,
			1,	0,
			0,	1,
			1,	1,
		};

		m_QuadData = new ShaderBuffer_POS_COL_UV(v1, v2, v3);
		m_VaoHandle = m_QuadData->getVAOHandle();
	}

	void draw(glsl_data& data) {}

	void draw(glsl_data& data, ShaderProgram*& shader, glm::vec3 position)
	{
		glBindVertexArray(m_VaoHandle);

		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::translate(glm::mat4(1.0f), position);
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));

		glActiveTexture(GL_TEXTURE0 + m_T.getUniformID());
		glBindTexture(GL_TEXTURE_2D, m_T.getTextureID());
		shader->setUniform("u_var_tex", m_T.getTextureID());

		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~E_textured_quad()
	{
		m_QuadData->deleteResource();
		m_T.deleteResource();
		delete m_QuadData;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }
	
	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
};

#endif