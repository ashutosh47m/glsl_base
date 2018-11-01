#pragma once
/*
GLSL_base
Oct 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _FBO_SS_SCATTER_ 
#define _FBO_SS_SCATTER_ 
#include "../resources/shader_program.h"
#include "../tools/variables.h"
#include "framebuffer.h"

// this class is nothing but an FBO, with loaded information
class FBOLightScatter
{
public:
	glm::vec3				m_lightPosOnSS;
	glm::mat4				m_MVP;
	// set this to 1 for no downsampling
	int						m_downsample = 1;

	struct scatterSetting
	{
		int sam; float exp, dec, den, wei;
	}
	// setting presets for god rays.
	// to increase performance set to low
		High{				150,	0.894000f,	0.968803f,	0.956968f,	0.208002f },
		Medium{				100,	0.987f,		0.952f,		0.960f,		0.400f, },
		Low{				50,		0.899999f,	0.933296f,	0.603005f,	0.454999f, },
		defaultScatter{		80,		0.6f,		0.93f,		.96f,		0.4f },
		Test{ }, off{ 0, };

	scatterSetting			m_current = High;

	variable<int>			m_numSamples = variable<int>("Sam", m_current.sam, 6, 1000);
	variable<float>			m_exposure = variable<float>("exp", m_current.exp);
	variable<float>			m_decay = variable<float>("dec", m_current.dec, 0, 1.0f);
	variable<float>			m_density = variable<float>("den", m_current.den);
	variable<float>			m_weight = variable<float>("wei", m_current.wei); // weight will be more for objects like sun

	FrameBuffer			   *m_FBO;

	void setGodRayUniforms(ShaderProgram *&fx)
	{
		glUseProgram(fx->getShaderProgramHandle());

		fx->setUniform("u_NUM_SAMPLES", m_numSamples.getValue());
		fx->setUniform("u_exposure", m_exposure.getValue());
		fx->setUniform("u_decay", m_decay.getValue());
		fx->setUniform("u_density", m_density.getValue());
		fx->setUniform("u_weight", m_weight.getValue());
	}

	FBOLightScatter(int w, int h, GLuint &globalTextureCount, ShaderProgram *& fx, GLuint MRTtextureID)
	{
		m_FBO = new FrameBuffer(w, h, m_downsample, globalTextureCount, false);

		// the information which needs to be updated only once should be sent here.
		setGodRayUniforms(fx);

		// MRTtextureID + 0 : default colore albedo texture which will be written from the MRT.  
		// MRTtextureID + 1 : will represent the lights in the scene for which we need god rays.

		// interesting fact: if you were to use MRTtextureID + 0 instead of 1, you will see some interesting effects.
		fx->setUniform("u_RT1_tex", MRTtextureID);
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

		// comment the following line when you're done editing the godray params
		setGodRayUniforms(shaderfx);

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
	}

	~FBOLightScatter()
	{
#ifdef _DEBUG
		// this code will write the values of light scatter before quitting in a file.

		FILE * f;
		fopen_s(&f, "scattersetting.txt", "w");
		fprintf(f, "%d\n%f\n%f\n%f\n%f\n", m_numSamples.getValue(), m_exposure.getValue(),
			m_decay.getValue(), m_density.getValue(), m_weight.getValue());
		fclose(f);
		delete m_FBO;

#endif
	}
};


#endif