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
	bool				 m_isWireframe;
public:
	E_mesh() {}

	void initEntity()
	{}

	void initEntity(std::string model_location, std::string texture_path)
	{
		m_isWireframe = false;
		m_mesh = new mesher::meshLoader(model_location.c_str(), texture_path.c_str());
	}
		
	void setWireframe(bool w)
	{
		m_isWireframe = w; 
	}

	void draw(glsl_data& data) {}

	void draw(glsl_data& data, ShaderProgram *& shader, glm::vec3 position)
	{
		glUseProgram(shader->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::translate(glm::mat4(1.0f), position);
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(2));
		
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);

		m_mesh->draw(shader, m_isWireframe);
	}

	void draw(glsl_data& data, ShaderProgram *& shader, glm::mat4 modelmat)
	{
		glUseProgram(shader->getShaderProgramHandle());
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelmat);
		m_mesh->draw(shader, m_isWireframe);
	}

	glm::mat4 getModelMatrix() { return m_ModelMat; }
	void enable()
	{
		//glBindVertexArray(m_VaoHandle);
	}
};

#endif