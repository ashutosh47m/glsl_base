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
		if (m_RTData != NULL)
		{
			m_RTData->deleteResource();
			delete m_RTData;
		}
	}
};

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

// after the multi-render pass post-process is used for rendering the desired framebuffer object onto the screen
// this is what user will see.
// this contents the rt_quad data
class PostProcess
{
	rt_quad		m_RTQuad;

public:
	PostProcess() {}

	void initData(GLuint &vaohandle)
	{
		m_RTQuad.initEntity();
		vaohandle = m_RTQuad.getVaoHandle();
	}

	void draw(ShaderProgram *& shader, GLuint textureID, GLuint texture, glm::mat4& model)
	{
		glUseProgram(shader->getShaderProgramHandle());

		glActiveTexture(GL_TEXTURE0 + textureID);
		glBindTexture(GL_TEXTURE_2D, texture);
		shader->setUniform("u_RT1_tex", textureID);

		shader->setUniform("u_m4MVP", model);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glUseProgram(0);
	}

	void CombinedDraw(
		ShaderProgram *& shader, 
		GLuint textureID1, GLuint texture1, 
		GLuint textureID2, GLuint texture2, 
		glm::mat4& model)
	{
		glUseProgram(shader->getShaderProgramHandle());

		glActiveTexture(GL_TEXTURE0 + textureID1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shader->setUniform("u_godraysIntermediateTex", textureID1);

		glActiveTexture(GL_TEXTURE0 + textureID2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader->setUniform("u_colorTex", textureID2);

		shader->setUniform("u_m4MVP", model);
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

// this class is nothing but an FBO, with loaded information
class FBOLightScatter
{
public:
	glm::vec3				m_lightPosOnSS;
	glm::mat4				m_MVP;
	// set this to 1 for no downsampling
	int						m_downsample = 2; 

	struct scatterSetting
	{
		int sam; float exp, dec, den, wei;
	}
	// setting presets for god rays.
	// to increase performance set to low
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
		0.987f,
		0.952f,
		0.960f,
		0.400f,
	},
	Low
	{
		50,
		0.899999f,
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

	scatterSetting			m_current		= High;

	variable<int>			m_numSamples 	= variable<int>  ("Sam", m_current.sam, 6, 1000);
	variable<float>			m_exposure 		= variable<float>("exp", m_current.exp);
	variable<float>			m_decay 		= variable<float>("dec", m_current.dec, 0, 1.0f);
	variable<float>			m_density 		= variable<float>("den", m_current.den);
	variable<float>			m_weight 		= variable<float>("wei", m_current.wei); // weight will be more for objects like sun

	FrameBuffer			   *m_FBO;

	FBOLightScatter(int w, int h, GLuint &globalTextureCount, ShaderProgram *& fx, GLuint MRTtextureID) 
	{
		m_FBO = new FrameBuffer(w, h, m_downsample, globalTextureCount);

		// the information which needs to be updated only once should be sent here.
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

	// this function is only for the debug purposes
#if _DEBUG
	void resetSettings()
	{
		m_numSamples.setValue(m_current.sam);
		m_exposure.setValue(m_current.exp);
		m_decay.setValue(m_current.dec);
		m_density.setValue(m_current.den);
		m_weight.setValue(m_current.wei);
	}
#endif

	void sendLightPositionForScatter(ShaderProgram*& shaderfx, GLuint textureID, glm::mat4& mvp_model, glm::mat4 mvp_lightposition)
	{
		glUseProgram(shaderfx->getShaderProgramHandle());
		shaderfx->setUniform("u_m4MVP", mvp_model);

		m_lightPosOnSS =
		(
			glm::vec3(
			mvp_lightposition[0][3] / mvp_lightposition[3][3], 
			mvp_lightposition[1][3] / mvp_lightposition[3][3], 
			mvp_lightposition[2][3] / mvp_lightposition[3][3]))
			* 0.5f
			+ glm::vec3(0.5f, 0.5f, 0.5f
		);
		shaderfx->setUniform("u_lightPos", glm::vec2(m_lightPosOnSS.x, m_lightPosOnSS.y));
		//printf("%f %f \n, ", m_lightPosOnSS.x, m_lightPosOnSS.y);
		shaderfx->setUniform("u_NUM_SAMPLES", m_numSamples.getValue());
		shaderfx->setUniform("u_exposure", m_exposure.getValue());
		shaderfx->setUniform("u_decay", m_decay.getValue());
		shaderfx->setUniform("u_density", m_density.getValue());
		shaderfx->setUniform("u_weight", m_weight.getValue());
	}

	~FBOLightScatter()
	{
#ifdef _DEBUG
// this code will write the values of light scatter before quitting in a file.

		FILE * f;
		fopen_s(&f,"scattersetting.txt", "w");
		fprintf(f, "%d\n%f\n%f\n%f\n%f\n", m_numSamples.getValue(), m_exposure.getValue(),
			m_decay.getValue(), m_density.getValue(), m_weight.getValue());
		fclose(f);
		delete m_FBO;

#endif
	}
};

/*
	E_fxMRT class will be used by the scene to call all the post processing functions.
	this class will hold the configuration/setting needed for post process.

	This class also creates multiple render targets for deferred rendering.
*/ 
class E_fxMRT
{
	GLuint					 m_VaoHandle;				// this is a vao handle for rt_quad 
	MRTFrameBuffer			*m_MRTFrameBuffer	= NULL;
	PostProcess				 postprocess;
	glm::mat4				 m_ModelMat;
	
	struct fxGlobalSettings
	{
		bool	global_postprocess;
		bool	godrays;
	};
	
public:

	fxGlobalSettings		 fx{ true, true};
	FBOLightScatter			*m_LightScatter = NULL;
	variable<GLfloat>		 m_ZPosition; 				// the position of the render target, u can move it closer to eye, or away from it

	E_fxMRT() {}

	void initEntity(GLuint &globalTextureCount, int w, int h, ShaderLibrary* lib, glsl_data &data)
	{
		m_ZPosition.setValue(DEFAULT_ZPOSITION_FOR_RENDER_TARGET);

		// once m_ModelMat is set for render targets its going to be constant throughout the life of a program.
		// coz we hardly change the render target of a game
		m_ModelMat = data.glm_model;
		m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(16.0f, 9.0f, 1.0f));
		m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, m_ZPosition.getValue()));

		m_MRTFrameBuffer	= new MRTFrameBuffer (w, h, globalTextureCount);
		m_LightScatter		= new FBOLightScatter(w, h, globalTextureCount, 
											lib->fx_lightscatter, 
											m_MRTFrameBuffer->m_MRTTextureID);

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

	void postProcessPass(glsl_data& data, ShaderLibrary* shaderLib, glm::mat4 &mvp_lightposition)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(m_VaoHandle); // this is a vaoHandle for the render target quad

		m_MRTFrameBuffer->activateMRTTextures();
#ifdef _DEBUG 
		m_MRTFrameBuffer->drawDebugRenderTargets(data, shaderLib->fx_rendertarget);
#endif		
		if (fx.godrays)
		{
			// light scatter aka god rays ------------------------------------------------------
			m_LightScatter->sendLightPositionForScatter
			(
				shaderLib->fx_lightscatter,
				m_MRTFrameBuffer->m_MRTTextureID + 1,
				data.glm_projection * data.getDefaultEye() * m_ModelMat, // mvp for render target
				data.glm_projection * data.glm_view * mvp_lightposition  // mvp for light
			);

			// commenting line below will increase performance, this calls the shader which creates god rays using sampling
			m_LightScatter->m_FBO->renderToTexture();
			//----------------------------------------------------------------------------------		
			postprocess.CombinedDraw
			(
				shaderLib->fx_combineLightscatter,											  // shader which combines 2 shaders
				m_LightScatter->m_FBO->m_MRTTextureID, m_LightScatter->m_FBO->m_ColorTexture, // processed god rays texture
				m_MRTFrameBuffer->m_MRTTextureID, m_MRTFrameBuffer->m_ColorTexture[0],		  // color texture 
				data.glm_projection * data.getDefaultEye() * m_ModelMat						  // modelViewMatrix
			);
		}
		else
		{
			// draw the normal color buffer gotten from the MRT
			postprocess.draw
			(
				shaderLib->fx_rendertarget,
				m_MRTFrameBuffer->m_MRTTextureID, m_MRTFrameBuffer->m_ColorTexture[0],
				data.glm_projection * data.getDefaultEye() * m_ModelMat
			);
		}
	}

	~E_fxMRT()
	{
		delete m_LightScatter;
		delete m_MRTFrameBuffer;
	}
	GLuint getVAOHandle() { return m_VaoHandle; }

	void enable()
	{
		//glBindVertexArray(m_VaoHandle);
	}
};


#endif