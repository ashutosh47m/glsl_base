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
	ShaderBuffer		*m_CubeData=NULL;
	GLuint				 m_VaoHandle;
	glm::mat4			 m_ModelMat;
	texture3D			 m_T3d;
	std::string			 m_TextureName;
	int					 m_Xdim, m_Ydim, m_Zdim;
public:
	E_volume_ray_caster() {}

	void initEntity() {}
	void initEntity(GLuint globalTextureCount, std::string volume_name, int xdm, int ydm, int zdm)
	{
		m_Xdim = xdm;
		m_Ydim = ydm;
		m_Zdim = zdm;

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

		m_CubeData = new ShaderBuffer_POS_IND(v1, v2);
		m_VaoHandle = m_CubeData->getVAOHandle();

		m_TextureName = "u_volume";
		m_T3d = texture3D(globalTextureCount, volume_name, m_TextureName, xdm, ydm, zdm);
	}

	void draw(glsl_data& data, ShaderProgram *& shader, glm::vec3 cam_position)
	{
		glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(m_VaoHandle);
			glUseProgram(shader->getShaderProgramHandle());
			
			m_ModelMat = data.glm_model;
			m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			shader->setUniform("u_step_size", 1.0f / m_Xdim, 1.0f / m_Ydim, 1.0f / m_Zdim);
			shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
			shader->setUniform("u_camPos", cam_position);
			glActiveTexture(GL_TEXTURE0 + m_T3d.getUniformID());
			glBindTexture(GL_TEXTURE_3D, m_T3d.getTextureID());
			shader->setUniform(m_T3d.getUniformVar(), m_T3d.getUniformID());
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		glDisable(GL_BLEND);
	}

	glm::mat4 getModelMatrix() { return m_ModelMat; }

	void enable()
	{
		//glBindVertexArray(m_VaoHandle);
	}

	~E_volume_ray_caster()
	{
		if (m_CubeData)
		{
			m_CubeData->deleteResource();
			m_T3d.deleteResource();
			delete m_CubeData;
		}
	}
};

#endif