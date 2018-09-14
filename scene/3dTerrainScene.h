#pragma once
/*
GLSL_base
Sept 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef TERSCN
#define TERSCN

#include "scene_handler.h"
#include "../Entity/E_mesh.h"

#include "../resources/framebuffer_handler.h"
#include "../resources/texture_handler.h"
#include "../tools/camera.h"
#include "../tools/keys.h"
#include "../tools/meshLoader.h"

namespace ter_scene
{
	class modelscene : public AbstractScene
{
	E_fxMRT						mE_fxmainRT;

	int							m_width, m_height;
	YP_Camera					*mT_camera;			// t stands for tools, i.e. YP_Camera comes under tools category
	glm::vec3					mViewDirection;
	glm::mat4					m_ModelMat;
	bool						_enableDebugCam;
	glm::vec3					_debugCamPosition = glm::vec3(4, 4, 4);
	E_mesh						mE_palm;

	bool						wasd[4] = { false };

	void initAxes();
	void initialize();
	void update();
	void draw();
	void renderWorld();

public:
	void keyProcess(int key, int scancode, int action, int mods);

	modelscene()
	{

	}
	~modelscene()
	{
		if (!mT_camera) delete mT_camera;
	}

	void setResolution(int w, int h)
	{
		m_width = w;
		m_height = h;
	}
};
}
#endif
