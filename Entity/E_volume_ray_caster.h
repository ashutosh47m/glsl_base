/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _E_VOL_RAY_CASTER
#define _E_VOL_RAY_CASTER

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"
#include "Entity.h"

class E_volume_ray_caster : public Entity
{
	ShaderBuffer_POS_IND cube_data;
	glm::mat4			 modelMat;
	texture3D			 t3d;
	std::string			 m_textureName;
	int					 xdim, ydim, zdim;
public:
	E_volume_ray_caster() {}

	void initEntity() {}
	void initEntity(GLuint globalTextureCount, std::string volume_name, int xdm, int ydm, int zdm)
	{
		xdim = xdm;
		ydim = ydm;
		zdim = zdm;

		std::vector<float> v1 =
		{
			-0.5f,  -0.5f,  -0.5f,
			0.5f,  -0.5f,  -0.5f,
			0.5f,   0.5f,  -0.5f,
			-0.5f,   0.5f,  -0.5f,
			-0.5f,  -0.5f,   0.5f,
			0.5f,  -0.5f,   0.5f,
			0.5f,   0.5f,   0.5f,
			-0.5f,   0.5f,   0.5f
		};

		std::vector<GLushort> v2 =
		{
			0,5,4,
			5,0,1,
			3,7,6,
			3,6,2,
			7,4,6,
			6,4,5,
			2,1,3,
			3,1,0,
			3,0,7,
			7,0,4,
			6,5,2,
			2,5,1
		};

		cube_data = ShaderBuffer_POS_IND(v1, v2);

		m_textureName = "u_volume";
		t3d = texture3D(globalTextureCount, volume_name, m_textureName, xdm, ydm, zdm);
	}

	void draw(glsl_data& data, ShaderProgram *& shader, glm::vec3 cam_position)
	{
		glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(cube_data.getVAOHandle());
			glUseProgram(shader->getShaderProgramHandle());
			modelMat = data.glm_model;
			shader->setUniform("u_step_size", 1.0f / xdim, 1.0f / ydim, 1.0f / zdim);
			shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * modelMat);
			shader->setUniform("u_camPos", cam_position);
			glBindTexture(GL_TEXTURE_3D, t3d.getTextureID());
			glActiveTexture(GL_TEXTURE0 + t3d.getUniformID());
			shader->setUniform(t3d.getUniformVar(), t3d.getUniformID());
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		glDisable(GL_BLEND);
	}

	void enable()
	{
		//glBindVertexArray(cube_data.getVAOHandle());
	}
};

#endif