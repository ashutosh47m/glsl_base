/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _FBO_
#define _FBO_

#include "..\Entity\Entity.h"
#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"

class FrameBuffer
{

};
	
class E_render_target
{
	ShaderBuffer			*m_RTData = NULL;
	GLuint					 m_VaoHandle;
	glm::mat4				 m_ModelMat;
	texture				 	 m_T;

public:

	E_render_target() {}

	void initEntity(GLuint globalTextureCount, std::string textureName)
	{
		m_T = texture(globalTextureCount, "u_var_tex", textureName);

		std::vector <float> v1 =
		{
			// what should be the ideal size of the quad you're rendering for RT
			.25, -.25, .50,
			-.25, -.25, .50,
			.25,  .25, .50,
			-.25,  .25, .50,
		};

		std::vector<float> v2 =
		{
			0,	0,
			1,	0,
			0,	1,
			1,	1,
		};

		m_RTData = new ShaderBuffer_POS_UV(v1, v2);
		m_VaoHandle = m_RTData->getVAOHandle();
	}

	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glBindVertexArray(m_VaoHandle);
			glBindTexture(GL_TEXTURE_2D, m_T.getTextureID());
			glActiveTexture(GL_TEXTURE0 + m_T.getUniformID());
			glUseProgram(shader->getShaderProgramHandle());
			//shader->setUniform("u_var_tex", m_T.getUniformID());
			m_ModelMat = data.glm_model;
			m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(3.5, 3, 1));
			m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0,0,1));
			shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~E_render_target()
	{
		m_RTData->deleteResource();
		delete m_RTData;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}

};
#endif