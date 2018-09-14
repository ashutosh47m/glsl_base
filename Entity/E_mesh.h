#pragma once
/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _E_MESH
#define _E_MESH

#include "Entity.h"
#include "../resources/shader_program.h"
#include "../tools/meshLoader.h"

class E_mesh : public Entity
{
	glm::mat4			 m_ModelMat;
	mesher::meshLoader	*m_mesh;
public:
	E_mesh() {}

	void initEntity() 
	{
		m_mesh = new mesher::meshLoader("..\\resources\\models\\vegetation\\palm\\palm_04.obj",
			"..\\resources\\models\\vegetation\\palm\\");
	}

	void draw(glsl_data& data) {}

	void draw(glsl_data& data, ShaderProgram *& shader, glm::vec3 position)
	{
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::translate(glm::mat4(1.0f), position);
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);

		m_mesh->draw(shader);
	}

	void enable()
	{
		//glBindVertexArray(m_VaoHandle);
	}
};

#endif