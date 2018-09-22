/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _ETRIANGLE
#define _ETRIANGLE

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "Entity.h"

class E_first_triangle : public Entity
{
	ShaderBuffer	 *m_TriangleData = NULL;
	glm::mat4		  m_ModelMat;
	GLuint			  m_VaoHandle;
public:
	E_first_triangle()
	{
	}

	void initEntity() 
	{
		std::vector<float> v1 =
		{
			.25, -.25, .50,
			-.25, -.25, .50,
			.25,  .25, .50
		};

		// when this code is put inside the constructor, the access violation happens  WHY?
		m_TriangleData = new ShaderBuffer_POS(v1);
		m_VaoHandle = m_TriangleData->getVAOHandle();
	}

	void draw(glsl_data& data, ShaderProgram *& shader) 
	{
		glBindVertexArray(m_VaoHandle);
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0));
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	~E_first_triangle() 
	{ 
		m_TriangleData->deleteResource();
		delete m_TriangleData;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
};


#endif