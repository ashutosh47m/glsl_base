
/*
	GLSL_base
	Nov 2017, Ashutosh Morwal
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/
#include "glrenderer.h"
#include <cstdio>

using namespace opengl_renderer;

#define DEFAULT_FOV 1.0472f

void renderer::initGL()
{
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

void renderer::resizeGL(int w, int h)
{
	printf("%d %d \n", w,h);
	if(h == 0)
		h = 1;
	glViewport (0, 0, w, h);

	// the first param of perspective projection function is the FoV.
	// ideally its represented in degrees, which can vary from 45-120, but in this case we represent it 
	// using radians, so converting 60 degrees <--> radians comes out to be 1.0472f

	//near plane can NOT be zero. if it is zero, then u will observe flickering																				
	default_scn.data.glm_projection = glm::perspective(1.5708f,  ((float)w/ (float)h), 0.0001f, 1000.0f);
	//default_scn.data.glm_projection = glm::ortho(0, w, h, 0, 0, 1000);
	
	//use following aspect ratio for 1280/720 resolution
	//default_scene.data.glm_projection = glm::perspective(49.0f, 2.6f, 0.1f, 8000.0f);
}
