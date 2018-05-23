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

// used by post-process and debugRTDraw to draw on
class rt_quad
{
	GLuint			m_VaoHandle;
	ShaderBuffer   *m_RTData = NULL;
public:
	void initEntity()
	{
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

	GLuint getVaoHandle()
	{
		return m_VaoHandle;
	}

	~rt_quad()
	{
		m_RTData->deleteResource();
		delete m_RTData;
	}
};

// multiple framebuffers
class MRTFrameBuffer
{
public:
	glm::mat4				 m_ModelMat;
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	static const GLuint		 m_MRTCount = 4; 	// cannot be less than 1, as 1 render target is must for rendering
	GLuint					 m_ColorTexture[m_MRTCount];
	GLuint					 m_MRTTextureID;
	float					 m_debugRenderTargetPosition = 3.3f;
	std::vector<GLenum>	 	 m_drawBuffers;

	MRTFrameBuffer() {}

	MRTFrameBuffer(GLuint w, GLuint h, GLuint &globalTextureCount) :
		m_Width(w),
		m_Height(h)
	{
		m_MRTTextureID = globalTextureCount;
		globalTextureCount += m_MRTCount;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(m_MRTCount, m_ColorTexture);
		glGenTextures(1, &m_DepthTexture);

		for (GLuint i = 0; i < m_MRTCount; i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorTexture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
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

	void drawDebugRenderTargets(glsl_data& data, ShaderProgram *& shader)
	{
		float j = 0;
		glUseProgram(shader->getShaderProgramHandle()); 

		for (GLuint i = 0; i < m_MRTCount; i++, j -= 1.1f)
		{
			shader->setUniform("u_RT1_tex", m_MRTTextureID + i);

			m_ModelMat = data.glm_model;
			m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(1.6f, .9f, 1.0f));
			m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(m_debugRenderTargetPosition, m_debugRenderTargetPosition + j, 1));
			shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * m_ModelMat);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}
};

// after the multi-render pass post-process is used for rendering the desired framebuffer object onto the screen
// this is what user will see.
class PostProcess
{
	GLuint		m_VaoHandle;	// vao handle used for RT quad
	glm::mat4	m_ModelMat;
	float		m_ZPosition;
	rt_quad		m_RTQuad;

public:
	PostProcess() {}
	PostProcess(GLuint vaohandle) : m_VaoHandle(vaohandle)
	{}

	void initData(GLuint &vaohandle, float zPosition)
	{
		m_RTQuad.initEntity();
		m_VaoHandle = m_RTQuad.getVaoHandle();
		vaohandle = m_VaoHandle;
		m_ZPosition = zPosition;
	}

	void draw(glsl_data& data, ShaderProgram *& shader, GLuint textureID, GLuint texture)
	{
		glUseProgram(shader->getShaderProgramHandle());
		glBindVertexArray(m_VaoHandle);

		glActiveTexture(GL_TEXTURE0 + textureID);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader->setUniform("u_RT1_tex", textureID);

		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_ZPosition));
		shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * m_ModelMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glUseProgram(0);
	}

	void CombinedDraw(glsl_data& data, ShaderProgram *& shader, GLuint textureID1, GLuint texture1, GLuint textureID2, GLuint texture2 )
	{
		glUseProgram(shader->getShaderProgramHandle());
		glBindVertexArray(m_VaoHandle);

		glActiveTexture(GL_TEXTURE0 + textureID1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shader->setUniform("u_godraysIntermediateTex", textureID1);

		glActiveTexture(GL_TEXTURE0 + textureID2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader->setUniform("u_colorTex", textureID2);

		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_ZPosition));
		shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * m_ModelMat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glUseProgram(0);
	}
};

// single frame buffer
class FrameBuffer
{
public:
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_ColorTexture;
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	GLuint					 m_MRTTextureID;
	float					 m_ZPosition;
	glm::mat4				 m_ModelMat;
	FrameBuffer() {}

	FrameBuffer(GLuint w, GLuint h, GLuint &globalTextureCount, float zpos) :
		m_Width(w),
		m_Height(h), 
		m_ZPosition(zpos)
	{
		m_MRTTextureID = ++globalTextureCount;

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


	void bindFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, draw_buffers);
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

// this class is going to create multiple framebuffers for one draw call.
class E_fxMRT
{
	GLuint					 m_VaoHandle;				// this is a vao handle for rt_quad 
	GLfloat					 m_ZPosition; 				// the position of the render target, u can move it closer to eye, or away from it
	MRTFrameBuffer			*m_MRTFrameBuffer	= NULL;
	FrameBuffer				*m_fboGrayscale		= NULL;
	FrameBuffer				*m_fboLightScatter	= NULL;
	PostProcess				 postprocess;
	glm::mat4				 m_ModelMat;

	// needed for light calculations
	glm::mat4				 __mvp;			
	glm::vec3				 lightOnSS;					// position of light in the screen space
public:

	E_fxMRT() {}

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
		m_ZPosition = 2.0f;

		m_MRTFrameBuffer	= new MRTFrameBuffer(w, h, globalTextureCount);
		m_fboGrayscale		= new FrameBuffer	(w, h, globalTextureCount, m_ZPosition);
		m_fboLightScatter	= new FrameBuffer	(w, h, globalTextureCount, m_ZPosition);

		postprocess.initData(m_VaoHandle, m_ZPosition);
	}

	void bindFBOForDraw()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_MRTFrameBuffer->m_ID);

		glDrawBuffers(m_MRTFrameBuffer->m_MRTCount, &m_MRTFrameBuffer->m_drawBuffers[0]);

		// if size of viewport for FBO is different than mainRT size then change the viewport
		//glViewport(0, 0, m_FBOWidth, m_FBOHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void unBindFBO()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// reset the viewport
		//glViewport(0, 0, m_FBOWidth, m_FBOHeight);
	}

	void sendLightPositionForScatter(glsl_data& data, ShaderProgram *& shaderfx, GLuint textureID)
	{
		glUseProgram(shaderfx->getShaderProgramHandle());
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_ZPosition));
		shaderfx->setUniform("u_RT1_tex", textureID);

		__mvp = data.glm_projection * data.getDefaultEye() * m_ModelMat;
		shaderfx->setUniform("u_m4MVP", __mvp);

		// this is not the place for calculating light position
		lightOnSS =
			(glm::vec3(__mvp[0][3] / __mvp[3][3], __mvp[1][3] / __mvp[3][3], __mvp[2][3] / __mvp[3][3]))
			* 0.5f
			+ glm::vec3(0.5f, 0.5f, 0.5f);

		shaderfx->setUniform("lightPos", lightOnSS);
	}

	void draw(glsl_data& data, ShaderLibrary* shaderLib)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(m_VaoHandle);
		
		m_MRTFrameBuffer->activateMRTTextures();
#ifdef _DEBUG 
		m_MRTFrameBuffer->drawDebugRenderTargets(data, shaderLib->fx_rendertarget);
#endif		
		// light scatter aka god rays
		sendLightPositionForScatter(data, shaderLib->fx_lightscatter, m_MRTFrameBuffer->m_MRTTextureID + 1);
		m_fboLightScatter->renderToTexture();
		
		//postprocess.draw(data, shaderLib->fx_rendertarget, m_fboLightScatter->m_MRTTextureID, m_fboLightScatter->m_ColorTexture);
		postprocess.CombinedDraw(data, shaderLib->fx_combineLightscatter, 
			m_fboLightScatter->m_MRTTextureID, m_fboLightScatter->m_ColorTexture,
			m_MRTFrameBuffer->m_MRTTextureID, m_MRTFrameBuffer->m_ColorTexture[0]);

	}

	~E_fxMRT()
	{
		delete m_fboLightScatter;
		delete m_fboGrayscale;
		delete m_MRTFrameBuffer;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
};


#endif