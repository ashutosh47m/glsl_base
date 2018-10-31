#pragma once
/*
GLSL_base
Oct 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _MRT_FBO_
#define _MRT_FBO_

#include "../resources/shader_program.h"
#include "../tools/variables.h"

// multiple framebuffers
class MRTFrameBuffer
{
public:
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	// the number of render targets this MRT is going to have. 
	// this value cannot be less than 1, as 1 render target is must for rendering	; 	
	static const GLuint		 m_MRTCount = 4;

	GLuint					 m_ColorTexture[m_MRTCount];
	GLuint					 m_MRTTextureID;

	// only for debug
	float					 m_debugRenderTargetPosition = 3.3f;

	std::vector<GLenum>	 	 m_drawBuffers;
	glm::mat4				 m_modelRT;

	MRTFrameBuffer() {}

	MRTFrameBuffer(GLuint w, GLuint h, GLuint &globalTextureCount) :
		m_Width(w),
		m_Height(h)
	{
		// we increase the global texture count based on the number of RTs we have
		m_MRTTextureID = globalTextureCount;
		globalTextureCount += m_MRTCount;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(m_MRTCount, m_ColorTexture);
		globalTextureCount++;
		glGenTextures(1, &m_DepthTexture);

		for (GLuint i = 0; i < m_MRTCount; i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorTexture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_ColorTexture[i], 0);

			m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT, m_Width, m_Height);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~MRTFrameBuffer()
	{
		glDeleteTextures(m_MRTCount, m_ColorTexture);
		glDeleteTextures(1, &m_DepthTexture);
		glDeleteFramebuffers(1, &m_ID);
	}

	void activateMRTTextures()
	{
		for (GLuint i = 0; i < m_MRTCount; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_ColorTexture[i]);
		}
	}

	// just a small function that draws debug render targets for visuazlization on screen
	void drawDebugRenderTargets(glsl_data& data, ShaderProgram *& shader)
	{
		float j = 0;
		glUseProgram(shader->getShaderProgramHandle());

		for (GLuint i = 0; i < m_MRTCount; i++, j -= 1.1f)
		{
			shader->setUniform("u_RT1_tex", m_MRTTextureID + i);

			m_modelRT = data.glm_model;
			m_modelRT *= glm::scale(glm::mat4(1.0f), glm::vec3(1.6f, .9f, 1.0f));
			m_modelRT *= glm::translate(glm::mat4(1.0f), glm::vec3(m_debugRenderTargetPosition, m_debugRenderTargetPosition + j, 1));
			shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * m_modelRT);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}
};

#endif