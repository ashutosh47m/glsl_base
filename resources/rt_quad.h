#pragma once
/*
GLSL_base
Oct 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _RT_Q_
#define _RT_Q_

#include "../resources/buffer_handler.h"
#include "../resources/shader_program.h"

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

#endif