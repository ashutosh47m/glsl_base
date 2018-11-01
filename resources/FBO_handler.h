/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _FBO_
#define _FBO_

#include "../scene/Entity/Entity.h"
#include "../resources/shader_program.h"
#include "../resources/MRT_Framebuffer.h"
#include "../resources/post_process.h"
#include "../resources/light_scatter.h"



#define DEFAULT_ZPOSITION_FOR_RENDER_TARGET 2.0f  
#define MRT_COLOR_ALBEDO 0
#define MRT_WHITE_GODRAYS 1

// used by post-process and debugRTDraw to draw on
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

	PostProcess& getPostProcessObject() { return postprocess; }

	S_light* getLights() { return postprocess.getLights(); }
	
	GLuint getLightCount() { return postprocess.getLightCount(); }

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

		postprocess.initData(m_VaoHandle, w, h, globalTextureCount);
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
		// comment the function when you're done editing the godray params
		m_ZPosition.update(1);
		
		m_LightScatter->m_numSamples.update(1);
		m_LightScatter->m_exposure.update(.001f);
		m_LightScatter->m_decay.update(.0001f);
		m_LightScatter->m_density.update(.001f);
		m_LightScatter->m_weight.update(.001f);
		
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
				m_MRTFrameBuffer->m_MRTTextureID + 1,					 // god ray white extract, basically your sun.
				data.glm_projection * data.getDefaultEye() * m_ModelMat, // mvp for render target
				data.glm_projection * data.glm_view * mvp_lightposition  // mvp for light
			);
			m_LightScatter->m_FBO->renderToTexture();					// this will process the godray texture
		}
		postprocess.DeferredDraw
		(
			shaderLib->fx_deferred_ADS,
			m_MRTFrameBuffer->m_MRTTextureID, m_MRTFrameBuffer->m_ColorTexture[1],
			m_MRTFrameBuffer->m_MRTTextureID + 1, m_MRTFrameBuffer->m_ColorTexture[2],
			m_MRTFrameBuffer->m_MRTTextureID + 2, m_MRTFrameBuffer->m_ColorTexture[3],
			data.glm_projection * data.getDefaultEye() * m_ModelMat,
			fx.godrays
		);

		if (fx.godrays)
		{
			//----------------------------------------------------------------------------------		
			postprocess.CombinedDraw
			(
				shaderLib->fx_combineLightscatter,											  // shader which combines 2 shaders
				m_LightScatter->m_FBO->m_MRTTextureID, m_LightScatter->m_FBO->m_ColorTexture, // processed god rays texture
				postprocess.m_DeferredFBO->m_MRTTextureID, postprocess.m_DeferredFBO->m_ColorTexture, // lit colored texture
				data.glm_projection * data.getDefaultEye() * m_ModelMat						  // modelViewMatrix
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