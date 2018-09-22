/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#ifndef _E_REDTRIANGLE
#define _E_REDTRIANGLE


#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "Entity.h"

class E_first_red_triangle : public Entity
{
	glm::mat4				tri_model_mat;
public:
	E_first_red_triangle()
	{ }

	void initEntity() 
	{
	}

	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glUseProgram(shader->getShaderProgramHandle());
		tri_model_mat = data.glm_model;
		tri_model_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));
		tri_model_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * tri_model_mat);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	~E_first_red_triangle()
	{
	}
	void enable()
	{
	}
};


#endif