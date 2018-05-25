#include "MRT_fboScene.h"

using namespace mrt_scene;

void MRTScene::initAxes()
{
	std::vector<float> pos_line = { -100,0,0, 100,0,0 };
	std::vector<float> col_line = { 1,1,1,1,1,1 };

	mE_X_axes.initEntity(pos_line, col_line);
	pos_line = { 0,-100,0, 0,100,0 };
	mE_Y_axes.initEntity(pos_line, col_line);
	pos_line = { 0,0,100, 0,0,-100 };
	mE_Z_axes.initEntity(pos_line, col_line);
}

void MRTScene::initialize()
{
	loadShaders();
	globalTextureCount = 0;
	//render target
	// the globalTextureCount will be incremented inside mainRTs initEntity based on the count of textures for RT
	mE_fxmainRT.initEntity(globalTextureCount, m_width, m_height, getShaderLibrary());

	// inititlize the axes
	initAxes();

	//load camera
	mT_camera = new YP_Camera(m_width, m_height);
}

void MRTScene::update()
{
	glEnable(GL_DEPTH_TEST);

	//by default back faces of objects are NOT hidden
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glClearColor(.5f, .515f, .515f, 1);
	mT_camera->cam_control(wasd, data.glm_eye, data.glm_view, mViewDirection);
}

void MRTScene::renderWorld()
{
	// we need this clear color to change the color inside the main RT
	glClearColor(.0f, .0f, .0f, 1);

	glLineWidth(10.0f);
	mE_X_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Y_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Z_axes.draw(data, getShaderLibrary()->colored_geometry);
}

void MRTScene::draw()
{
	mE_fxmainRT.bindFBOForDraw();
		renderWorld();
	mE_fxmainRT.unBindFBO();

//	mE_fxmainRT.draw(data, getShaderLibrary()->rendertarget);
	glBindVertexArray(0);
}

void MRTScene::keyProcess(int key, int scancode, int action, int mods)
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