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
	ShaderProgram *triangle_shader;
	ShaderBuffer_POS triangle_data;
	glm::mat4 tri_model_mat;
public:
	E_first_triangle()
	{
	}

	void initEntity() 
	{
		// when this code is put inside the constructor, the access violation happens 
		//triangle_shader = new ShaderProgram("../shaders/first_triangle");
		triangle_data = {
						 .25, -.25, .50, 
						-.25, -.25, .50, 
						 .25,  .25, .50
						};
	}

	void draw(glsl_data& data, ShaderProgram *& shader) 
	{
		glBindVertexArray(triangle_data.getVAOHandle());
		glUseProgram(shader->getShaderProgramHandle());
		tri_model_mat = data.glm_model;
		tri_model_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, 0));
		tri_model_mat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * tri_model_mat);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//ShaderProgram* getShader()
	//{ return triangle_shader; }

	~E_first_triangle() 
	{ 
		//if(triangle_data !=NULL)
		//	delete triangle_data;
		triangle_data.deleteResource();
		//if (triangle_shader != NULL)
		//	delete triangle_shader;
	}
	GLuint getVAOHandle() { return triangle_data.getVAOHandle(); }

	void enable()
	{
		glBindVertexArray(triangle_data.getVAOHandle());
	}
};


#endif