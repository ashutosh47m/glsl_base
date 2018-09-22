/*
	GLSL_base
	Nov 2017, Ashutosh Morwal 
	@Email ashutosh47m@gmail.com
	@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "..\scene\Scene_test.h"
#include "..\scene\Scene_flat_terrain.h"
#include "..\scene\Scene_MRT_fbo.h"
namespace opengl_renderer
{
	class renderer
	{
		//usr_scene::TestScene test_scene;
		//AbstractScene &default_scn = test_scene;
		
		//mrt_scene::GodRayTestScene	mrt_scn;
		//AbstractScene &default_scn = mrt_scn;

		ter_scene::modelscene _3dTerrain_scene;
		AbstractScene &default_scn = _3dTerrain_scene;

	public:
		renderer() {}
		~renderer() {}

		//usr_scene::TestScene* getScene() { return &test_scene; }
		//mrt_scene::GodRayTestScene* getScene() { return &mrt_scn; }
		ter_scene::modelscene* getScene() { return &_3dTerrain_scene; }

		void initGL();
		void resizeGL(int w, int h);
		void updateGL(); //update the state of the objects.. and timer
		void displayGL();
		void shutdownGL();
	};
}
#endif

