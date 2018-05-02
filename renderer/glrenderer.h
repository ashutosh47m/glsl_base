/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "..\scene\testScene.h"
namespace opengl_renderer
{
	class renderer
	{
		usr_scene::TestScene test_scene;
		AbstractScene &default_scn = test_scene;

	public:
		renderer() {}
		~renderer() {}

		usr_scene::TestScene getScene() { return test_scene; }

		void initGL();
		void resizeGL(float w, float h);
		void updateGL(); //update the state of the objects.. and timer
		void displayGL();
		void shutdownGL();
	};
}
#endif

