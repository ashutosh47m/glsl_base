/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef TESTSCN
#define TESTSCN

#include "scene_handler.h"

#include "../Entity/E_origin_axis.h"
#include "../Entity/E_first_triangle.h"
#include "../Entity/E_first_red_triangle.h"
#include "../Entity/E_colored_quad.h"
#include "../Entity/E_textured_quad.h"
#include "../Entity/E_textured_triangle.h"
#include "../Entity/E_volume.h"
#include "../Entity/E_cube.h"
#include "../Entity/E_volume_ray_caster.h"

#include "../resources/texture_handler.h"
#include "../tools/camera.h"
#include "../tools/keys.h"

namespace usr_scene
{
	class TestScene : public AbstractScene
	{
		E_first_triangle			mE_triangle;			// e stands for entity
		E_textured_triangle			mE_texturedTriangle;
		E_first_red_triangle		mE_red_triangle;
		E_colored_quad				mE_quad;
		E_textured_quad				mE_Woodenquad;
		E_textured_quad				mE_Marblequad;
		E_textured_quad				mE_grassStonequad;
		E_colored_line				mE_X_axes, mE_Y_axes, mE_Z_axes;
		//E_3d_texture_volume		mE_vol;
		E_cube						mE_cube;
		E_volume_ray_caster			mE_vrc; // volume ray caster

		int							m_width, m_height;
		YP_Camera					*mT_camera;			// t stands for tools, i.e. YP_Camera comes under tools category
		glm::vec3					mViewDirection;

		bool						wasd[4] = { false };
	
		void initAxes();
		void initialize();
		void update();
		void draw();

public: 
		void keyProcess(int key, int scancode, int action, int mods);

		TestScene()
		{

		}
		~TestScene()
		{
		}

		void setResolution(int w, int h)
		{
			m_width = w;
			m_height = h;
		}
	};
};

#endif
