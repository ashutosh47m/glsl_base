
/*
	GLSL_base
	Nov 2017, Ashutosh Morwal
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/
#include "glrenderer.h"
#include <cstdio>

using namespace opengl_renderer;

void renderer::initGL()
{
	glewInit();
	glEnable    (GL_DEPTH_TEST);
	glEnable    (GL_BLEND);
	glEnable	(GL_TEXTURE_2D);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable	(GL_MULTISAMPLE);	
	ilInit();

	default_scn.initialize();
}

void renderer::displayGL()
{
	default_scn.draw();
}

void renderer::updateGL()
{
	default_scn.update();
}

void renderer::shutdownGL()
{
}

void renderer::resizeGL(float w, float h)
{
	printf("%f %f \n", w,h);
	if(h == 0)
		h = 1;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	default_scn.data.glm_projection = glm::perspective(60.0f, (float) (w/h), 0.1f, 8000.0f);

	//use following aspect ratio for 1280/720 resolution
	//default_scene.data.glm_projection = glm::perspective(49.0f, 2.6f, 0.1f, 8000.0f);
}
