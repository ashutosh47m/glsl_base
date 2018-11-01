#pragma once
/*
GLSL_base
Nov 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _FBO_SHADOW_MAP_
#define _FBO_SHADOW_MAP_

#include "framebuffer.h"
#include "../shader_program.h"

class FBOShadow_map
{
	GLuint shadow_width, shadow_height;
	GLuint window_width, window_height;
public:
	glm::mat4				m_MVP;
	int						m_downsample = 1;
	FrameBuffer			   *m_FBO;

	FBOShadow_map(int w, int h, GLuint &globalTextureCount, ShaderProgram *& fx, GLuint MRTtextureID)
	{
		window_width = w;
		window_height = h;
		m_FBO = new FrameBuffer(w, h, m_downsample, globalTextureCount, true);
		fx->setUniform("u_RT1_tex", MRTtextureID);
	}

	void setShadowWidth(int multiplier)
	{
		shadow_width	= window_width * multiplier;
		shadow_height	= window_height * multiplier;
	}

	void enableDepthCapture()
	{
		glViewport(0, 0, shadow_width, shadow_height);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO->m_ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_FBO->m_ID, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void disableDepthCapture()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CULL_FACE);

		glViewport(0, 0, window_width, window_height);
	}

	~FBOShadow_map()
	{
		delete m_FBO;
	}
};
#endif