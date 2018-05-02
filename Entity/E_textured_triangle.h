/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _E_TXTRD_TRNGL
#define _E_TXTRD_TRNGL


#include "Entity.h"
#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "../resources/texture_handler.h"

class E_textured_triangle : public Entity
{
	ShaderProgram			*textured_geometry;
	ShaderBuffer_POS_COL_UV triangle_data;
	glm::mat4				modelMat;
	texture					t;

public:
	E_textured_triangle() {}
	void initEntity()  {}

	void initEntity(GLuint globalTextureCount, std::string textureName) 
	{
		//textured_geometry = new ShaderProgram("../shaders/textured_geometry");

		t = texture(globalTextureCount, "u_var_tex", textureName);

		std::vector<float> v1 = 
		{ 
			.25, -.25, .50,
			-.25, -.25, .50,
			.25,  .25, .50,
		};
		std::vector<float> v2 =
		{
			1,	0, 0,
			0,	1, 0,
			0,	0, 1,
		};

		std::vector<float> v3 =
		{
			0,	0,
			1,	0,
			0,	1,
			1,	1,
		};

		triangle_data = ShaderBuffer_POS_COL_UV(v1, v2, v3);
	}

	void draw(glsl_data& data)  {}

	void draw(glsl_data& data, glm::vec3 position, ShaderProgram *& shader)
	{
		glUseProgram(shader->getShaderProgramHandle());
		glBindTexture(GL_TEXTURE_2D, t.getTextureID());
		modelMat = data.glm_model;
		modelMat *= glm::translate(glm::mat4(1.0f), position);
		modelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_var_tex", t.getUniformID());
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//ShaderProgram* getShader()
	//{
	//	return textured_geometry;
	//}

	~E_textured_triangle()
	{
		//if(triangle_data !=NULL)
		//	delete triangle_data;
		triangle_data.deleteResource();
		t.deleteResource();
		//if (textured_geometry != NULL)
		//	delete textured_geometry;
	}
	GLuint getVAOHandle() { return triangle_data.getVAOHandle(); }

};

#endif