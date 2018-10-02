#include "Scene_test.h"

using namespace usr_scene;

void TestScene::initAxes()
{
	std::vector<float> pos_line = { -100,0,0, 100,0,0 };
	std::vector<float> col_line = { 1,1,1,1,1,1};
	
	mE_X_axes.initEntity(pos_line, col_line);
	pos_line = { 0,-100,0, 0,100,0 };
	mE_Y_axes.initEntity(pos_line, col_line);
	pos_line = { 0,0,100, 0,0,-100 };
	mE_Z_axes.initEntity(pos_line, col_line);
}

void TestScene::initialize()
{
	loadShaders();
	global2DTextureCount = 0;
	global3DTextureCount = 0;
	//render target
	// the globalTextureCount will be incremented inside mainRTs initEntity based on the count of textures for RT
	if (mE_fxmainRT.fx.global_postprocess)
		mE_fxmainRT.initEntity(global2DTextureCount, m_width, m_height, getShaderLibrary(), data);


	// inititlize the axes
	initAxes();

	mE_brickTriangle.initEntity		(global2DTextureCount, "..\\resources\\textures\\brick.jpg");
	//quad
	mE_Junglequad.initEntity		(global2DTextureCount, "..\\resources\\textures\\jungle.jpg");
	mE_Woodenquad.initEntity		(global2DTextureCount, "..\\resources\\textures\\wooden.jpg");
	mE_grassStonequad.initEntity	(global2DTextureCount, "..\\resources\\textures\\green.jpg");
	
	mE_vrc.initEntity				(global3DTextureCount, "..\\resources\\volumes\\head256x256x109\\head256x256x109.raw", 256,256,109);
	
	//cube			
	mE_cube.initEntity(true);
	mE_ColoredSkybox.initEntity(false);
	
	mE_palm.initEntity("palm", "..\\resources\\models\\vegetation\\palm\\palm_04.obj", "..\\resources\\models\\vegetation\\palm\\");

	_enableDebugCam = false;
	//load camera
	if(!_enableDebugCam)
		mT_camera = new YP_Camera(m_width, m_height);
	else 
		data.setViewMatrix(_debugCamPosition);
}

void TestScene::update()
{
	glEnable(GL_DEPTH_TEST);

	//by default back faces of objects are NOT hidden
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glClearColor(.0f, .0f, .0f, 1);
	if (_enableDebugCam)
		data.setViewMatrix(_debugCamPosition);
	else
		mT_camera->cam_control(wasd, data.glm_eye, data.glm_view, mViewDirection);
}

void TestScene::renderWorld()
{
	// we need this clear color to change the color inside the main RT
	glClearColor(.0f, .0f, .0f, 1);

	mE_vrc.draw(data, getShaderLibrary()->volume_ray_caster, data.glm_eye);

	//glDisable(GL_CULL_FACE);
		
	mE_Woodenquad.draw		(data, getShaderLibrary()->textured_colored_geometry,	glm::vec3( -1.1, 0, 0));
	mE_brickTriangle.draw	(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(-2.2, 0, 0));
	mE_Junglequad.draw		(data, getShaderLibrary()->textured_colored_geometry,	glm::vec3(  1.1, 0, 0));
	mE_grassStonequad.draw	(data, getShaderLibrary()->textured_colored_geometry,	glm::vec3(  2.2, 0, 0));
	//glEnable(GL_CULL_FACE);

	//mE_X_axes.draw(data, getShaderLibrary()->sun);
	//mE_Y_axes.draw(data, getShaderLibrary()->sun);
	//mE_Z_axes.draw(data, getShaderLibrary()->sun);
	
	mE_palm.draw(data, getShaderLibrary()->mesh, glm::vec3(0, -0, 0));

	m_ModelMat = data.glm_model;
	m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(1224, 1224, 1224));
	mE_ColoredSkybox.draw(data, getShaderLibrary()->sun, m_ModelMat);

	mE_cube.draw(data, getShaderLibrary()->sun, glm::vec3(0, 2, 2));
	mE_cube.draw(data, getShaderLibrary()->sun, glm::vec3(0, 1, -2));
	mE_cube.draw(data, getShaderLibrary()->sun, glm::vec3(-2, 0, 0));
	mE_cube.draw(data, getShaderLibrary()->sun, glm::vec3(2, -1, 0));
	
	glBindVertexArray(0);
}

void TestScene::draw()
{
	if(mE_fxmainRT.fx.global_postprocess)
	{ 
		mE_fxmainRT.update();

		mE_fxmainRT.bindFBOForDraw();
			renderWorld();
		mE_fxmainRT.unBindFBO();
	
		mE_fxmainRT.postProcessPass(data, getShaderLibrary(), mE_vrc.getModelMatrix());
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderWorld();
	}
}

void TestScene::keyProcess(int key, int scancode, int action, int mods)
{
	if (action == _2am_KEY_PRESS)
	{
		switch (key)
		{
		case _2am_KEY_W:
			if(_enableDebugCam)
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
			if(mT_camera->getDebugCam())
				mT_camera->setDebugCam(false);
			else
				mT_camera->setDebugCam(true);
			break;
		case _2am_KEY_Z:
			_debugCamPosition.y--;
			break;

		case _2am_KEY_R: mE_fxmainRT.m_ZPosition.Toggle = 1; break;
		case _2am_KEY_F: mE_fxmainRT.m_ZPosition.Toggle = 2; break;

		case _2am_KEY_T: mE_fxmainRT.m_LightScatter->m_numSamples.Toggle = 1; break;
		case _2am_KEY_G: mE_fxmainRT.m_LightScatter->m_numSamples.Toggle = 2; break;

		case _2am_KEY_Y: mE_fxmainRT.m_LightScatter->m_exposure.Toggle = 1; break;
		case _2am_KEY_H: mE_fxmainRT.m_LightScatter->m_exposure.Toggle = 2; break;

		case _2am_KEY_U: mE_fxmainRT.m_LightScatter->m_decay.Toggle = 1; break;
		case _2am_KEY_J: mE_fxmainRT.m_LightScatter->m_decay.Toggle = 2; break;

		case _2am_KEY_I: mE_fxmainRT.m_LightScatter->m_density.Toggle = 1; break;
		case _2am_KEY_K: mE_fxmainRT.m_LightScatter->m_density.Toggle = 2; break;

		case _2am_KEY_O: mE_fxmainRT.m_LightScatter->m_weight.Toggle = 1; break;
		case _2am_KEY_L: mE_fxmainRT.m_LightScatter->m_weight.Toggle = 2; break;

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
		case _2am_KEY_R:
		case _2am_KEY_F:
			mE_fxmainRT.m_ZPosition.Toggle = 0;
			break;
		case _2am_KEY_T:
		case _2am_KEY_G:
			mE_fxmainRT.m_LightScatter->m_numSamples.Toggle = 0;
			break;

		case _2am_KEY_Y:
		case _2am_KEY_H:
			mE_fxmainRT.m_LightScatter->m_exposure.Toggle = 0;
			break;

		case _2am_KEY_U:
		case _2am_KEY_J:
			mE_fxmainRT.m_LightScatter->m_decay.Toggle = 0;
			break;
			
		case _2am_KEY_I:
		case _2am_KEY_K:
			mE_fxmainRT.m_LightScatter->m_density.Toggle = 0;
			break;
			
		case _2am_KEY_O:
		case _2am_KEY_L:
			mE_fxmainRT.m_LightScatter->m_weight.Toggle = 0;
			break;
		case _2am_KEY_SPACE:
			// reset changed uniform params for light scatter
			//mE_fxmainRT.m_LightScatter->resetSettings();
			break;
		}

	}
	else if (action == _2am_KEY_REPEAT)
	{
	}
}