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
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_Width, m_Height);

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

class MRTFrameBuffer
{
public:
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	GLuint					 m_UniformID;		// the global texture ID
	static const GLuint		 m_MRTCount = 4;
	GLuint					 m_ColorTexture[m_MRTCount];
	MRTFrameBuffer() {}

	MRTFrameBuffer(GLuint w, GLuint h, GLuint uni) :
		m_Width(w),
		m_Height(h),
		m_UniformID(uni)
	{
		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(m_MRTCount, m_ColorTexture);
		glGenTextures(1, &m_DepthTexture);

		for (GLuint i = 0; i < m_MRTCount; i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorTexture[i]);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_Width, m_Height);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, m_ColorTexture[i], 0);
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
};

class E_render_target
{
	ShaderBuffer			*m_RTData = NULL;
	GLuint					 m_VaoHandle;
	glm::mat4				 m_ModelMat;
	GLfloat					 m_ZPosition; 			// the position of the render target, u can move it closer to eye, or away from it
	//FrameBuffer			*m_FrameBuffer = NULL;	// the FBO object
	MRTFrameBuffer			*m_MRTFrameBuffer = NULL;
	GLuint					 m_MRTTextureID;
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

	void initEntity(GLuint &globalTextureCount, int w, int h)
	{
		//m_FrameBuffer = new FrameBuffer(w, h, globalTextureCount);
		m_MRTFrameBuffer = new MRTFrameBuffer(w, h, globalTextureCount);

		m_MRTTextureID = globalTextureCount;
		globalTextureCount += m_MRTFrameBuffer->m_MRTCount;
		
		m_ZPosition = -.25f;
		

		std::vector <float> v1 =
		{
			 .5, -.5, .5,
			-.5, -.5, .5,
			 .5,  .5, .5,
			-.5,  .5, .5,
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
		//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer->m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_MRTFrameBuffer->m_ID);
		GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(m_MRTFrameBuffer->m_MRTCount, draw_buffers);


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

	void setRTAtDefaultPosition(glsl_data& data, ShaderProgram *& shader)
	{
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_ZPosition));
		shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * m_ModelMat);
	}

	void setRTAtDebugPosition(glsl_data& data, ShaderProgram *& shader, glm::vec3 position)
	{
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), position);
		shader->setUniform("u_m4MVP", data.glm_projection * data.glm_view * m_ModelMat);
	}

	void draw(glsl_data& data, ShaderProgram *& shader)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(m_VaoHandle);
			glUseProgram(shader->getShaderProgramHandle());

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_MRTFrameBuffer->m_ColorTexture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_MRTFrameBuffer->m_ColorTexture[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_MRTFrameBuffer->m_ColorTexture[2]);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_MRTFrameBuffer->m_ColorTexture[3]);

			//glBindTexture(GL_TEXTURE_2D, m_FrameBuffer->m_ColorTexture);
			//glActiveTexture(GL_TEXTURE0 + m_FrameBuffer->m_UniformID);
			
			shader->setUniform("u_RT1_tex", m_MRTTextureID);
			setRTAtDebugPosition(data, shader, glm::vec3(.59, 0.61, 1));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			shader->setUniform("u_RT1_tex", m_MRTTextureID + 1);
			setRTAtDebugPosition(data, shader, glm::vec3(.59, -0.61, 1));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			shader->setUniform("u_RT1_tex", m_MRTTextureID + 2);
			setRTAtDebugPosition(data, shader, glm::vec3(-.59, -.61, 1));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			shader->setUniform("u_RT1_tex", m_MRTTextureID + 3);
			setRTAtDebugPosition(data, shader, glm::vec3(-.59,  .61, 1));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glBindTexture(GL_TEXTURE_2D, 0);
	}

	~E_render_target()
	{
		m_RTData->deleteResource();
		delete m_RTData;
		//delete m_FrameBuffer;
		delete m_MRTFrameBuffer;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}

};
#endif