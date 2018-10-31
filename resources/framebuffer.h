#pragma once
/*
GLSL_base
Oct 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#include "../resources/shader_program.h"

#ifndef _SINGLE_FBO_
#define _SINGLE_FBO_

// single frame buffer
class FrameBuffer
{
public:
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_ColorTexture;
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	GLuint					 m_MRTTextureID;
	GLuint					 m_Wdownsampled, m_Hdownsampled;

	FrameBuffer() {}

	FrameBuffer(GLuint w, GLuint h, int downsample, GLuint &globalTextureCount) :
		m_Width(w),
		m_Height(h)
	{
		m_Wdownsampled = w / downsample;
		m_Hdownsampled = h / downsample;

		m_MRTTextureID = ++globalTextureCount;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		//	change GL_RGBA8 to GL_RGBA32 for better results
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_Wdownsampled, m_Hdownsampled);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &m_DepthTexture);
		globalTextureCount++;
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT, m_Wdownsampled, m_Hdownsampled);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);

		static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, draw_buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void bindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		// in FBO when only one color attachment is present, you do not need to explicitely call glDrawBuffers 
			//static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
			//glDrawBuffers(1, draw_buffers);
		glViewport(0, 0, m_Wdownsampled, m_Hdownsampled);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void renderToTexture()
	{
		bindFBO();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		unbindFBO();
	}
	void unbindFBO()
	{
		glViewport(0, 0, m_Width, m_Height);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	~FrameBuffer()
	{
		glDeleteTextures(1, &m_ColorTexture);
		glDeleteTextures(1, &m_DepthTexture);
		glDeleteFramebuffers(1, &m_ID);
	}
};

#endif
