/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _FBO_
#define _FBO_

#include "../Entity/Entity.h"
#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"

class FrameBuffer
{
public:
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_ColorTexture;	
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	GLuint					 m_UniformID;		// the global texture ID

	FrameBuffer() {}

	FrameBuffer(GLuint w, GLuint h, GLuint uni) :
		m_Width (w), 
		m_Height(h), 
		m_UniformID(uni)
	{
		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		//	change GL_RGBA8 to GL_RGBA32 for better results
		glTexStorage2D(GL_TEXTURE_2D, 9, GL_RGBA32F, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexStorage2D(GL_TEXTURE_2D, 9, GL_DEPTH_COMPONENT32F, m_Width, m_Height);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorTexture, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);

		static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, draw_buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~FrameBuffer()
	{
		glDeleteTextures(1, &m_ColorTexture);
		glDeleteTextures(1, &m_DepthTexture);
		glDeleteFramebuffers(1, &m_ID);
	}
};
	
class E_render_target
{
	ShaderBuffer			*m_RTData = NULL;
	GLuint					 m_VaoHandle;
	glm::mat4				 m_ModelMat;
	GLfloat					 m_ZPosition; 			// the position of the render target, u can move it closer to eye, or away from it
	FrameBuffer				*m_FrameBuffer = NULL;	// the FBO object

public:

	E_render_target() {}

	void incrZPosition()
	{
		m_ZPosition += .01f;
		printf("m_ZPosition %f\n", m_ZPosition);
	}
	void decrZPosition()
	{
		m_ZPosition -= .01f;
		printf("m_ZPosition %f\n", m_ZPosition);
	}

	void initEntity(GLuint globalTextureCount, int w, int h)
	{
		m_ZPosition = -.25f;
		
		m_FrameBuffer = new FrameBuffer(w, h, globalTextureCount);

		std::vector <float> v1 =
		{
			 .25, -.25, .50,
			-.25, -.25, .50,
			 .25,  .25, .50,
			-.25,  .25, .50,
		};

		std::vector<float> v2 =
		{
			0,	0,
			1,	0,
			0,	1,
			1,	1,
		};

		m_RTData = new ShaderBuffer_POS_UV(v1, v2);
		m_VaoHandle = m_RTData->getVAOHandle();
	}

	void bindFBOForDraw()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer->m_ID);
		// if size of viewport for FBO is different than mainRT size then change the viewport
		//glViewport(0, 0, m_FBOWidth, m_FBOHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void unBindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// reset the viewport
		//glViewport(0, 0, m_FBOWidth, m_FBOHeight);
	}

	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(m_VaoHandle);
			glUseProgram(shader->getShaderProgramHandle());
			glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->m_ColorTexture);
			glActiveTexture(GL_TEXTURE0 + m_FrameBuffer->m_UniformID);
			//shader->setUniform("u_var_tex", m_FrameBuffer->m_UniformID);
			m_ModelMat = data.glm_model;
			m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9, 1));
			m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0,0, m_ZPosition));
			shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * m_ModelMat);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~E_render_target()
	{
		m_RTData->deleteResource();
		delete m_RTData;
		delete m_FrameBuffer;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}

};
#endif