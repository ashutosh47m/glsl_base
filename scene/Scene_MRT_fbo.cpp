#include "Scene_MRT_fbo.h"
#include "../scene/Entity/E_textured_quad.h"

using namespace mrt_scene;

void GodRayTestScene::initAxes()
{
	std::vector<float> pos_line = { -100,0,0, 100,0,0 };
	std::vector<float> col_line = { 1,1,1,1,1,1 };

	mE_X_axes.initEntity(pos_line, col_line);
	pos_line = { 0,-100,0, 0,100,0 };
	mE_Y_axes.initEntity(pos_line, col_line);
	pos_line = { 0,0,100, 0,0,-100 };
	mE_Z_axes.initEntity(pos_line, col_line);
}

void GodRayTestScene::initialize()
{
	loadShaders();
	global2DTextureCount = 0;
	//render target
	// the globalTextureCount will be incremented inside mainRTs initEntity based on the count of textures for RT
	mE_fxmainRT.initEntity(global2DTextureCount, m_width, m_height, getShaderLibrary(), data);
	
	// inititlize the axes
	initAxes();

	mE_ColoredSkybox.initEntity(false);
	mE_Marblequad.initEntity(++global2DTextureCount, "..\\resources\\textures\\jungle.jpg");

	//load camera
	mT_camera = new YP_Camera(m_width, m_height);
}

void GodRayTestScene::update()
{
	glEnable(GL_DEPTH_TEST);

	//by default back faces of objects are NOT hidden
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glClearColor(.5f, .515f, .515f, 1);
	mT_camera->cam_control(wasd, data.glm_eye, data.glm_view, mViewDirection);
}

void GodRayTestScene::renderWorld()
{
	// we need this clear color to change the color inside the main RT
	glClearColor(.0f, .0f, .0f, 1);
	
	m_ModelMat = data.glm_model;
	m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(24, 24, 24));
	mE_ColoredSkybox.draw(data, getShaderLibrary()->colored_geometry, m_ModelMat);

	glLineWidth(10.0f);
	mE_X_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Y_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Z_axes.draw(data, getShaderLibrary()->colored_geometry);

	mE_Marblequad.enable();
	mE_Marblequad.draw(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(0, 0,  0));
}

void GodRayTestScene::draw()
{
	if (mE_fxmainRT.fx.global_postprocess)
	{
		mE_fxmainRT.bindFBOForDraw();
		renderWorld();
		mE_fxmainRT.unBindFBO();

//		mE_fxmainRT.postProcessPass(data, getShaderLibrary());
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderWorld();
	}
}

void GodRayTestScene::keyProcess(int key, int scancode, int action, int mods)
{
	if (action == _2am_KEY_PRESS)
	{
		switch (key)
		{
		case _2am_KEY_W:
			wasd[0] = true;
			break;
		case _2am_KEY_A:
			wasd[1] = true;
			break;
		case _2am_KEY_S:
			wasd[2] = true;
			break;
		case _2am_KEY_D:
			wasd[3] = true;
			break;

		case _2am_KEY_R:
			//mE_fxmainRT.decrZPosition();
			break;

		case _2am_KEY_T:
			//mE_fxmainRT.incrZPosition();
			break;

		case _2am_KEY_UP: break;
		case _2am_KEY_DOWN: break;
		case _2am_KEY_RIGHT: break;
		case _2am_KEY_LEFT:	break;
		}
	}
	else if (action == _2am_KEY_RELEASE)
	{
		switch (key)
		{
		case _2am_KEY_W:
			wasd[0] = false;
			break;
		case _2am_KEY_A:
			wasd[1] = false;
			break;
		case _2am_KEY_S:
			wasd[2] = false;
			break;
		case _2am_KEY_D:
			wasd[3] = false;
			break;
		}
	}
	else if (action == _2am_KEY_REPEAT)
	{
	}
}