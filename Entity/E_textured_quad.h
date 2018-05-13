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
	ShaderBuffer_POS_COL_UV quad_data;
	glm::mat4				modelMat;
	texture					t;

public:
	E_textured_quad() {}
	void initEntity() {}

	void initEntity(GLuint globalTextureCount, std::string textureName) 
	{
		t = texture(globalTextureCount, "u_var_tex", textureName);

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

		quad_data = ShaderBuffer_POS_COL_UV(v1, v2, v3);
	}

	void draw(glsl_data& data) {}

	void draw(glsl_data& data, glm::vec3 position, ShaderProgram*& shader) 
	{
		glUseProgram(shader->getShaderProgramHandle());
		glBindTexture(GL_TEXTURE_2D, t.getTextureID());
		modelMat = data.glm_model;
		modelMat *= glm::translate(glm::mat4(1.0f), position);
		modelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_var_tex", t.getUniformID());
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~E_textured_quad()
	{
		quad_data.deleteResource();
		t.deleteResource();
	}
	GLuint getVAOHandle() { return quad_data.getVAOHandle(); }
	
	void enable()
	{
		glBindVertexArray(quad_data.getVAOHandle());
	}
};

#endif