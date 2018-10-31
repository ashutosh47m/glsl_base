#pragma once
/*
GLSL_base
Oct 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/
#include "../resources/rt_quad.h"
#include "../resources/framebuffer.h"

#ifndef _FBO_POST_PROCESS_
#define _FBO_POST_PROCESS_

// after the multi-render pass post-process is used for rendering the desired framebuffer object onto the screen
// this is what user will see.
// this contents the rt_quad data

struct S_light
{
	glm::vec3 position;
	glm::vec3 color;
}; 

class PostProcess
{
	rt_quad		m_RTQuad;
	glm::vec3	viewPosition;
	static const GLuint		 LIGHT_CNT = 32;

	S_light lights[LIGHT_CNT];
	float x, y = 30, z;

public:
	GLuint getLightCount() { return LIGHT_CNT; }
	S_light* getLights() { return lights; }
	FrameBuffer			   *m_DeferredFBO;
	PostProcess() {}
	~PostProcess() { delete m_DeferredFBO; }

	void setViewPosition(glm::vec3 p)
	{
		viewPosition = p;
	}

	void initData(GLuint &vaohandle, int w, int h, GLuint& texCnt)
	{
		m_DeferredFBO = new FrameBuffer(w, h, 1, texCnt);

		m_RTQuad.initEntity();
		vaohandle = m_RTQuad.getVaoHandle();
		for (int i = 0; i < LIGHT_CNT; i++)
		{
			x = 10.0f * cos(40.0f + (i * 40.0f));
			z = 10.0f * sin(40.0f + (i * 40.0f));

			lights[i].position = glm::vec3(x, y, z);
			lights[i].color = glm::vec3(0.08f, 0.04f, 0.04f);
		}
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

	void DeferredDraw(
		ShaderProgram *& shader,
		GLuint textureID1, GLuint texture1,
		GLuint textureID2, GLuint texture2,
		GLuint textureID3, GLuint texture3,
		glm::mat4& model,
		bool drawIntermediate // if enabled this will draw on intermediate FBO, else we'll draw on the screen
	)
	{
		glUseProgram(shader->getShaderProgramHandle());

		glActiveTexture(GL_TEXTURE0 + textureID1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		shader->setUniform("u_albedo", textureID1);

		glActiveTexture(GL_TEXTURE0 + textureID2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader->setUniform("u_position", textureID2);

		glActiveTexture(GL_TEXTURE0 + textureID3);
		glBindTexture(GL_TEXTURE_2D, texture3);
		shader->setUniform("u_normal", textureID3);

		shader->setUniform("u_m4MVP", model);
		shader->setUniform("u_v3ViewPos", viewPosition);

		for (int i = 0; i < LIGHT_CNT; i++)
		{
			std::string pos = "u_lights[" + std::to_string(i) + "].Position";
			std::string col = "u_lights[" + std::to_string(i) + "].Color";

			shader->setUniform(pos.c_str(), lights[i].position);
			shader->setUniform(col.c_str(), lights[i].color);
		}

		if (drawIntermediate)
			m_DeferredFBO->renderToTexture();
		else
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glUseProgram(0);
	}
};


#endif