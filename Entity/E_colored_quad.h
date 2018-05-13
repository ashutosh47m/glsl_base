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
	ShaderBuffer_POS_COL quad_data;
	glm::mat4 modelMat;

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

		quad_data = ShaderBuffer_POS_COL(v1, v2);
	}

	void draw(glsl_data& data, ShaderProgram *& shader) 
	{
		glBindVertexArray(quad_data.getVAOHandle());
		glUseProgram(shader->getShaderProgramHandle());
		modelMat = data.glm_model;
		modelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0));
		modelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~E_colored_quad()
	{
		quad_data.deleteResource();
	}
	GLuint getVAOHandle() { return quad_data.getVAOHandle(); }
	
	void enable()
	{
		glBindVertexArray(quad_data.getVAOHandle());
	}

};

#endif