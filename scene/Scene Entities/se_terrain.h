#pragma once
/*
GLSL_base
sept 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _SE_TERRAIN_
#define _SE_TERRAIN_

#include "../Entity/E_mesh.h"
#include "../../resources/texture_handler.h"

class se_terrain : public Entity
{
	E_mesh						 mE_terrain;
	texture_jpg			 		*m_Texdry;
public:
	E_mesh getMesh() { return mE_terrain; }
	void initEntity(
		std::string modelPath, std::string modelTexturePath,
		GLuint& globalTextureCount, 
		std::string textureName)
	{
		mE_terrain.initEntity(modelPath, modelTexturePath);
		m_Texdry = new texture_jpg(globalTextureCount, "u_dryTex", textureName);
	}

	void draw()
	{}

	void draw(glsl_data& data, ShaderProgram*& shader, glm::vec3 position)
	{
		glUseProgram(shader->getShaderProgramHandle());
		
		glActiveTexture(GL_TEXTURE0 + m_Texdry->getUniformID());
		glBindTexture(GL_TEXTURE_2D, m_Texdry->getTextureID());
		shader->setUniform("u_dryTex", m_Texdry->getTextureID());
		
		mE_terrain.draw(data, shader, position);
	}

	~se_terrain()
	{
		delete m_Texdry;
	}
	void enable()
	{}
};

#endif