#include "Scene_flat_terrain.h"

using namespace ter_scene;

void modelscene::initAxes()
{
}

void modelscene::initialize()
{
	loadShaders();
	global2DTextureCount = 0;
	global3DTextureCount = 0;
	//render target
	// the globalTextureCount will be incremented inside mainRTs initEntity based on the count of textures for RT
	if (mE_fxmainRT.getFXSettings().global_postprocess)
		mE_fxmainRT.initEntity(global2DTextureCount, m_width, m_height, getShaderLibrary(), data);

	mE_terrain.initEntity("..\\resources\\models\\terrain\\test_large.obj", "..\\resources\\models\\vegetation\\palm\\",
		global2DTextureCount, "..\\resources\\textures\\terrain\\soil2_CLR.jpg");
	mE_terrain.getMesh().setWireframe(false);

	mE_tree1.initEntity("8_2 tree", "..\\resources\\models\\vegetation\\8_2k.obj", "..\\resources\\models\\vegetation\\");
	mE_tree2.initEntity("1_1 tree", "..\\resources\\models\\vegetation\\1_1k_default.obj", "..\\resources\\models\\vegetation\\");
	mE_palm.initEntity("palm", "..\\resources\\models\\vegetation\\palm\\palm_04.obj", "..\\resources\\models\\vegetation\\palm\\");
	mE_palm.setCull(false);
	mE_sun.initEntity("sun", "..\\resources\\models\\sky\\sun.obj", "..\\resources\\models\\sky\\");

	mE_LightCube.initEntity(true);

	_enableDebugCam = false;
	//load camera
	if (!_enableDebugCam)
		mT_camera = new YP_Camera(m_width, m_height);
	else
		data.setViewMatrix(_debugCamPosition);
	//by default back faces of objects are NOT hidden
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void modelscene::update()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(.0f, .0f, .0f, 1);
	if (_enableDebugCam)
		data.setViewMatrix(_debugCamPosition);
	else
		mT_camera->cam_control(wasd, data.glm_eye, data.glm_view, mViewDirection);
}

void modelscene::draw()
{
	if (mE_fxmainRT.getFXSettings().global_postprocess)
	{
		mE_fxmainRT.update();

		mE_fxmainRT.bindFBOForDraw();
		renderWorld();
		mE_fxmainRT.unBindFBO();

		mE_fxmainRT.postProcessPass(data, getShaderLibrary(), mE_sun.getModelMatrix());
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderWorld();
	}
}

void modelscene::drawModels(ShaderProgram *& shader)
{
	mE_tree1.draw(data, shader, glm::vec3(4, 1.3, 20));
	mE_tree2.draw(data, shader, glm::vec3(5, -4, -20));

	mE_palm.draw(data, shader, glm::vec3(10, -4, 0));
	mE_palm.draw(data, shader, glm::vec3(10, -2, 10));
	mE_palm.draw(data, shader, glm::vec3(0, -5, 0));
	mE_palm.draw(data, shader, glm::vec3(0, -2, 10));
}

void modelscene::renderWorld()
{
	// we need this clear color to change the color inside the main RT
	glClearColor(.0f, .0f, .0f, 1);

	// commenting draw god ray sun
	// ToDo : bug 1 - the position where we're rendering SUN should match the position of light...
	//		  bug 2 - we need to consult framebuffer handler if we're using GOD RAYS, if we're using god rays then only we render the sun..	
	m_ModelMat = data.glm_model;
	m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(580,580,0));
	m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(87, 87, 88));
	mE_sun.draw(data, getShaderLibrary()->sun, m_ModelMat);

	drawModels(getShaderLibrary()->mesh);

	mE_terrain.draw(data, getShaderLibrary()->terrain, glm::vec3(0, -40, 0));

	slights = mE_fxmainRT.getLights();
	for (unsigned int i = 0; i < mE_fxmainRT.getLightCount(); i++)
	{
		mE_LightCube.draw(data, getShaderLibrary()->triangle_shader, slights[i].position);
	}
	glBindVertexArray(0);
}

void modelscene::keyProcess(int key, int scancode, int action, int mods)
{
	if (action == _2am_KEY_PRESS)
	{
		switch (key)
		{
		case _2am_KEY_W:
			if (_enableDebugCam)
				_debugCamPosition.z++;
			else
				wasd[0] = true;
			break;
		case _2am_KEY_A:
			if (_enableDebugCam)
				_debugCamPosition.x--;
			else
				wasd[1] = true;
			break;
		case _2am_KEY_S:
			if (_enableDebugCam)
				_debugCamPosition.z--;
			else
				wasd[2] = true;
			break;
		case _2am_KEY_D:
			if (_enableDebugCam)
				_debugCamPosition.x++;
			else
				wasd[3] = true;
			break;
		case _2am_KEY_Q:
			_debugCamPosition.y++;
			break;
		case _2am_KEY_C:
			if (mT_camera->getFastCam())
				mT_camera->setFastCam(false);
			else
				mT_camera->setFastCam(true);
			break;
		case _2am_KEY_Z:
			_debugCamPosition.y--;
			break;

		case _2am_KEY_R: mE_fxmainRT.m_ZPosition.Toggle = 1; break;
		case _2am_KEY_F: mE_fxmainRT.m_ZPosition.Toggle = 2; break;

		case _2am_KEY_T: mE_fxmainRT.getLightScatter()->m_numSamples.Toggle = 1; break;
		case _2am_KEY_G: mE_fxmainRT.getLightScatter()->m_numSamples.Toggle = 2; break;

		case _2am_KEY_Y: mE_fxmainRT.getLightScatter()->m_exposure.Toggle = 1; break;
		case _2am_KEY_H: mE_fxmainRT.getLightScatter()->m_exposure.Toggle = 2; break;

		case _2am_KEY_U: mE_fxmainRT.getLightScatter()->m_decay.Toggle = 1; break;
		case _2am_KEY_J: mE_fxmainRT.getLightScatter()->m_decay.Toggle = 2; break;

		case _2am_KEY_I: mE_fxmainRT.getLightScatter()->m_density.Toggle = 1; break;
		case _2am_KEY_K: mE_fxmainRT.getLightScatter()->m_density.Toggle = 2; break;

		case _2am_KEY_O: mE_fxmainRT.getLightScatter()->m_weight.Toggle = 1; break;
		case _2am_KEY_L: mE_fxmainRT.getLightScatter()->m_weight.Toggle = 2; break;

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
			if (!_enableDebugCam)
				mE_fxmainRT.getPostProcessObject().setViewPosition(mT_camera->getPosition());
			break;
		case _2am_KEY_A:
			if (!_enableDebugCam)
				mE_fxmainRT.getPostProcessObject().setViewPosition(mT_camera->getPosition());
			wasd[1] = false;
			break;
		case _2am_KEY_S:
			if (!_enableDebugCam)
				mE_fxmainRT.getPostProcessObject().setViewPosition(mT_camera->getPosition());
			wasd[2] = false;
			break;
		case _2am_KEY_D:
			if (!_enableDebugCam)
				mE_fxmainRT.getPostProcessObject().setViewPosition(mT_camera->getPosition());
			wasd[3] = false;
			break;
		case _2am_KEY_R:
		case _2am_KEY_F:
			mE_fxmainRT.m_ZPosition.Toggle = 0;
			break;
		case _2am_KEY_T:
		case _2am_KEY_G:
			mE_fxmainRT.getLightScatter()->m_numSamples.Toggle = 0;
			break;

		case _2am_KEY_Y:
		case _2am_KEY_H:
			mE_fxmainRT.getLightScatter()->m_exposure.Toggle = 0;
			break;

		case _2am_KEY_U:
		case _2am_KEY_J:
			mE_fxmainRT.getLightScatter()->m_decay.Toggle = 0;
			break;

		case _2am_KEY_I:
		case _2am_KEY_K:
			mE_fxmainRT.getLightScatter()->m_density.Toggle = 0;
			break;

		case _2am_KEY_O:
		case _2am_KEY_L:
			mE_fxmainRT.getLightScatter()->m_weight.Toggle = 0;
			break;
		case _2am_KEY_SPACE:
			// reset changed uniform params for light scatter
			mE_fxmainRT.getLightScatter()->resetSettings();
			break;
		}

	}
	else if (action == _2am_KEY_REPEAT)
	{
	}
}