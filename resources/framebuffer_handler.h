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
#include "../tools/variables.h"

#define DEFAULT_ZPOSITION_FOR_RENDER_TARGET 2.0f
#define MRT_COLOR_ALBEDO 0
#define MRT_WHITE_GODRAYS 1
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
	GLuint					 m_ID;				// used for generating the FBO
	GLuint					 m_DepthTexture;
	GLuint					 m_Width, m_Height;
	static const GLuint		 m_MRTCount = 2; 	// cannot be less than 1, as 1 render target is must for rendering
	GLuint					 m_ColorTexture[m_MRTCount];
	GLuint					 m_MRTTextureID;
	float					 m_debugRenderTargetPosition = 3.3f;
	std::vector<GLenum>	 	 m_drawBuffers;
	glm::mat4				 m_modelRT;

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

// after the multi-render pass post-process is used for rendering the desired framebuffer object onto the screen
// this is what user will see.
class PostProcess
{
	GLuint		m_VaoHandle;	// vao handle used for RT quad
	rt_quad		m_RTQuad;

public:
	PostProcess() {}
	PostProcess(GLuint vaohandle) : m_VaoHandle(vaohandle)
	{}

	void initData(GLuint &vaohandle)
	{
		m_RTQuad.initEntity();
		m_VaoHandle = m_RTQuad.getVaoHandle();
		vaohandle = m_VaoHandle;
	}

	void draw(glsl_data& data, ShaderProgram *& shader, GLuint textureID, GLuint texture, glm::mat4 model)
	{
		glUseProgram(shader->getShaderProgramHandle());
		glBindVertexArray(m_VaoHandle);

		glActiveTexture(GL_TEXTURE0 + textureID);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader->setUniform("u_RT1_tex", textureID);

		shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * model);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glUseProgram(0);
	}

	void CombinedDraw(glsl_data& data, ShaderProgram *& shader, GLuint textureID1, GLuint texture1, GLuint textureID2, GLuint texture2, glm::mat4 model)
	{
		glUseProgram(shader->getShaderProgramHandle());
		glBindVertexArray(m_VaoHandle);

		glActiveTexture(GL_TEXTURE0 + textureID1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shader->setUniform("u_godraysIntermediateTex", textureID1);

		glActiveTexture(GL_TEXTURE0 + textureID2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader->setUniform("u_colorTex", textureID2);

		shader->setUniform("u_m4MVP", data.glm_projection * data.getDefaultEye() * model);
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
	FrameBuffer() {}

	FrameBuffer(GLuint w, GLuint h, GLuint &globalTextureCount) :
		m_Width(w),
		m_Height(h) 
	{
		m_MRTTextureID = ++globalTextureCount;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		//	change GL_RGBA8 to GL_RGBA32 for better results
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &m_DepthTexture);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT, m_Width, m_Height);

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

// this class is nothing but an FBO, with loaded information
class FBOLightScatter
{
public:
	glm::vec3				m_lightPosOnSS;
	glm::mat4				m_MVP;

	struct scatterSetting
	{
		int sam; float exp, dec, den, wei;
	}
	// setting presets for god rays.
	High
	{
		 150,
		.894f,
		.968803f,
		.81997f,
		.414f
	},
	Medium
	{
		100,
		0.587f,
		0.952f,
		0.960f,
		0.400f,
	},
	Low
	{
		50,
		0.679999f,
		0.933296f,
		0.603005f,
		0.454999f,
	},
	Test
	{
	},
	defaultScatter
	{
		80,
		0.6f,
		0.93f,
		.96f,
		0.4f
	},
	off
	{
		0,
	};
	scatterSetting			m_current = High;

	variable<int>			m_numSamples 	= variable<int>  ("Sam", m_current.sam, 6, 1000);
	variable<float>			m_exposure 		= variable<float>("exp", m_current.exp);
	variable<float>			m_decay 		= variable<float>("dec", m_current.dec, 0, 1.0f);
	variable<float>			m_density 		= variable<float>("den", m_current.den);
	variable<float>			m_weight 		= variable<float>("wei", m_current.wei); // weight will be more for objects like sun

	FrameBuffer			   *m_FBO;
	FBOLightScatter(int w, int h, GLuint &globalTextureCount, ShaderProgram *& fx, GLuint MRTtextureID) 
	{
		m_FBO = new FrameBuffer(w, h, globalTextureCount);
		glUseProgram(fx->getShaderProgramHandle());

		fx->setUniform("u_NUM_SAMPLES", m_numSamples.getValue());
		fx->setUniform("u_exposure", m_exposure.getValue());
		fx->setUniform("u_decay", m_decay.getValue());
		fx->setUniform("u_density", m_density.getValue());
		fx->setUniform("u_weight", m_weight.getValue());

		// MRTtextureID + 0 : default colore albedo texture which will be written from the MRT.  
		// MRTtextureID + 1 : will represent the lights in the scene for which we need god rays.

		// interesting fact: if you were to use MRTtextureID + 0 instead of 1, you will see some interesting effects.
		fx->setUniform("u_RT1_tex", MRTtextureID + 1); 
	}

	void resetSettings()
	{
		m_numSamples.setValue(m_current.sam);
		m_exposure.setValue(m_current.exp);
		m_decay.setValue(m_current.dec);
		m_density.setValue(m_current.den);
		m_weight.setValue(m_current.wei);
	}

	void sendLightPositionForScatter(glsl_data& data, ShaderProgram *& shaderfx, GLuint textureID, glm::mat4 & model)
	{
		glUseProgram(shaderfx->getShaderProgramHandle());
		m_MVP = data.glm_projection * data.getDefaultEye() * model;
		shaderfx->setUniform("u_m4MVP", m_MVP);

		m_lightPosOnSS =
			(glm::vec3(m_MVP[0][3] / m_MVP[3][3], m_MVP[1][3] / m_MVP[3][3], m_MVP[2][3] / m_MVP[3][3]))
			* 0.5f
			+ glm::vec3(0.5f, 0.5f, 0.5f);
		shaderfx->setUniform("u_lightPos", glm::vec2(m_lightPosOnSS.x, m_lightPosOnSS.y));
	}

	~FBOLightScatter()
	{
#ifdef _DEBUG
		FILE * f;
		fopen_s(&f,"scattersetting.txt", "w");
		fprintf(f, "%d\n%f\n%f\n%f\n%f\n", m_numSamples.getValue(), m_exposure.getValue(),
			m_decay.getValue(), m_density.getValue(), m_weight.getValue());
		fclose(f);
		delete m_FBO;
#endif
	}
};

// this class is going to create multiple framebuffers for one draw call.
class E_fxMRT
{
	GLuint					 m_VaoHandle;				// this is a vao handle for rt_quad 
	MRTFrameBuffer			*m_MRTFrameBuffer	= NULL;
	PostProcess				 postprocess;
	glm::mat4				 m_ModelMat;
public:

	FBOLightScatter			*m_LightScatter = NULL;
	variable<GLfloat>		 m_ZPosition; 				// the position of the render target, u can move it closer to eye, or away from it

	E_fxMRT() {}

	void initEntity(GLuint &globalTextureCount, int w, int h, ShaderLibrary* lib, glm::mat4 &datamodel)
	{
		m_ZPosition.setValue(DEFAULT_ZPOSITION_FOR_RENDER_TARGET);

		m_ModelMat = datamodel;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_ZPosition.getValue()));

		m_MRTFrameBuffer	= new MRTFrameBuffer (w, h, globalTextureCount);
		m_LightScatter		= new FBOLightScatter(w, h, globalTextureCount, lib->fx_lightscatter, m_MRTFrameBuffer->m_MRTTextureID);

		postprocess.initData(m_VaoHandle);
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
		glUseProgram(0);
		// reset the viewport
		//glViewport(0, 0, m_FBOWidth, m_FBOHeight);
	}

	void update()
	{
		//m_ZPosition.update(1);
		/*
		m_LightScatter->m_numSamples.update(1);
		m_LightScatter->m_exposure.update(.001f);
		m_LightScatter->m_decay.update(.0001f);
		m_LightScatter->m_density.update(.001f);
		m_LightScatter->m_weight.update(.001f);
		*/
	}

	void postProcess(glsl_data& data, ShaderLibrary* shaderLib)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(m_VaoHandle); // this is a vaoHandle for the render target quad
		
		m_MRTFrameBuffer->activateMRTTextures();
#ifdef _DEBUG 
		m_MRTFrameBuffer->drawDebugRenderTargets(data, shaderLib->fx_rendertarget);
#endif		
		// light scatter aka god rays ------------------------------------------------------
		m_LightScatter->sendLightPositionForScatter(data, shaderLib->fx_lightscatter, m_MRTFrameBuffer->m_MRTTextureID + 1, m_ModelMat);
		m_LightScatter->m_FBO->renderToTexture();
		//----------------------------------------------------------------------------------		
		
//		postprocess.draw(data, shaderLib->fx_rendertarget, m_MRTFrameBuffer->m_MRTTextureID, m_MRTFrameBuffer->m_ColorTexture[0], m_ZPosition.getValue());
		postprocess.CombinedDraw(data, shaderLib->fx_combineLightscatter, 
			m_LightScatter->m_FBO->m_MRTTextureID, m_LightScatter->m_FBO->m_ColorTexture, // god rays 
			m_MRTFrameBuffer->m_MRTTextureID, m_MRTFrameBuffer->m_ColorTexture[0], m_ModelMat); // color texture
	}

	~E_fxMRT()
	{
		delete m_LightScatter;
		delete m_MRTFrameBuffer;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		glBindVertexArray(m_VaoHandle);
	}
};


#endif