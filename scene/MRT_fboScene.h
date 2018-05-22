/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _MRT_SCN_
#define _MRT_SCN_

#include "scene_handler.h"
#include "../resources/framebuffer_handler.h"
#include "../resources/texture_handler.h"
#include "../tools/camera.h"
#include "../tools/keys.h"
#include "../Entity/E_origin_axis.h"

namespace mrt_scene
{
	class MRTScene : public AbstractScene
	{
		E_fxMRT						mE_fxmainRT;
		int							m_width, m_height;
		YP_Camera					*mT_camera;			// t stands for tools, i.e. YP_Camera comes under tools category
		glm::vec3					mViewDirection;
	
		bool						wasd[4] = { false };
		E_colored_line				mE_X_axes, mE_Y_axes, mE_Z_axes;

		void initAxes();
		void initialize();
		void update();
		void draw();
		void renderWorld();

	public:
		void keyProcess(int key, int scancode, int action, int mods);

		MRTScene()
		{

		}
		~MRTScene()
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