/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef TESTSCN
#define TESTSCN

#include "scene_handler.h"

#include "../scene/Entity/E_origin_axis.h"
#include "../scene/Entity/E_first_triangle.h"
#include "../scene/Entity/E_first_red_triangle.h"
#include "../scene/Entity/E_colored_quad.h"
#include "../scene/Entity/E_textured_quad.h"
#include "../scene/Entity/E_textured_triangle.h"
#include "../scene/Entity/E_volume.h"
#include "../scene/Entity/E_cube.h"
#include "../scene/Entity/E_mesh.h"
#include "../scene/Entity/E_volume_ray_caster.h"

#include "../resources/postprocess/FBO_handler.h"
#include "../resources/texture_handler.h"
#include "../tools/camera.h"
#include "../tools/keys.h"

#include "../tools/meshLoader.h"

namespace usr_scene
{
	class TestScene : public AbstractScene
	{
		E_textured_triangle			mE_brickTriangle;

		E_textured_quad				mE_Woodenquad;
		E_textured_quad				mE_Junglequad;
		E_textured_quad				mE_grassStonequad;

		E_colored_line				mE_X_axes, mE_Y_axes, mE_Z_axes;

		E_mesh						mE_palm;
		//E_3d_texture_volume		mE_vol;


		E_cube						mE_cube;
		E_cube						mE_ColoredSkybox;

		E_volume_ray_caster			mE_vrc; // volume ray caster

		E_fxMRT						mE_fxmainRT;

		int							m_width, m_height;
		YP_Camera					*mT_camera;			// t stands for tools, i.e. YP_Camera comes under tools category
		glm::vec3					mViewDirection;
		glm::mat4					m_ModelMat;
		bool						_enableDebugCam;
		glm::vec3					_debugCamPosition = glm::vec3(4, 4, 4);

		bool						wasd[4] = { false };
	
		void initAxes();
		void initialize();
		void update();
		void draw();
		void renderWorld();

public: 
		void keyProcess(int key, int scancode, int action, int mods);

		TestScene()
		{

		}
		~TestScene()
		{
			if(!mT_camera) delete mT_camera;
		}

		void setResolution(int w, int h)
		{
			m_width = w;
			m_height = h;
		}
	};
};

#endif
