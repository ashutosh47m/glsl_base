#include "testScene.h"

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
	globalTextureCount = 0;
	//render target
	// the globalTextureCount will be incremented inside mainRTs initEntity based on the count of textures for RT
	mE_fxmainRT.initEntity(globalTextureCount, m_width, m_height);

	// triangles
	mE_triangle.initEntity();	//inits the shaders and other resources
	mE_red_triangle.initEntity();
	mE_texturedTriangle.initEntity	(++globalTextureCount, "..\\resources\\textures\\grassstone.jpg");

	// inititlize the axes
	initAxes();

	//quad
	mE_quad.initEntity();
	
	mE_Woodenquad.initEntity		(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");
	mE_Marblequad.initEntity		(++globalTextureCount, "..\\resources\\textures\\marble.jpg");
	mE_grassStonequad.initEntity	(++globalTextureCount, "..\\resources\\textures\\grassstone.jpg");
	
	//mE_vrc.initEntity				(++globalTextureCount, "..\\resources\\volumes\\engine256.raw", 256,256,256);
	mE_vrc.initEntity				(++globalTextureCount, "..\\resources\\volumes\\head256x256x109\\head256x256x109.raw", 256,256,109);
	
	//cube			//colored
	mE_cube.initEntity(true);
	mE_ColoredSkybox.initEntity(true);
	
	//load camera
	mT_camera = new YP_Camera(m_width, m_height);
}

void TestScene::update()
{
	glEnable(GL_DEPTH_TEST);

	//by default back faces of objects are NOT hidden
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glClearColor(.5f, .515f, .515f, 1);
	mT_camera->cam_control(wasd, data.glm_eye, data.glm_view, mViewDirection);
}

void TestScene::renderWorld()
{
	// we need this clear color to change the color inside the main RT
	glClearColor(.0f, .0f, .0f, 1);

	mE_vrc.draw(data, getShaderLibrary()->volume_ray_caster, data.glm_eye);

	//glDisable(GL_CULL_FACE);
		mE_triangle.draw(data, getShaderLibrary()->colored_geometry);
		mE_red_triangle.draw(data, getShaderLibrary()->red_triangle_shader);
		mE_texturedTriangle.draw(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(2, 1, 0));

		mE_quad.draw(data, getShaderLibrary()->colored_geometry, glm::vec3(4, 0, 0));

		mE_Woodenquad.enable();
		mE_Woodenquad.draw(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(-1, 1, 0));
		mE_Woodenquad.draw(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(-3, 1, 0));
		mE_Marblequad.draw(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(-1, 2, 0));
		mE_grassStonequad.draw(data, getShaderLibrary()->textured_colored_geometry, glm::vec3(1, 2, 0));
	//glEnable(GL_CULL_FACE);

	mE_X_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Y_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Z_axes.draw(data, getShaderLibrary()->colored_geometry);

	m_ModelMat = data.glm_model;
	m_ModelMat *= glm::scale(glm::mat4(1.0f), glm::vec3(24, 24, 24));
	m_ModelMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	mE_ColoredSkybox.draw(data, getShaderLibrary()->colored_geometry, m_ModelMat);

	mE_cube.draw(data, getShaderLibrary()->colored_geometry, glm::vec3(0, 2, 2));
	mE_cube.draw(data, getShaderLibrary()->colored_geometry, glm::vec3(0, 1, -2));
	mE_cube.draw(data, getShaderLibrary()->colored_geometry, glm::vec3(-2, 0, 0));
	mE_cube.draw(data, getShaderLibrary()->colored_geometry, glm::vec3(2, -1, 0));
}

void TestScene::draw()
{
	mE_fxmainRT.bindFBOForDraw();
		renderWorld();
	mE_fxmainRT.unBindFBO();
	
	mE_fxmainRT.draw(data, getShaderLibrary()->rendertarget, getShaderLibrary()->fx_lightscatter);
	glBindVertexArray(0);
}

void TestScene::keyProcess(int key, int scancode, int action, int mods)
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
			mE_fxmainRT.decrZPosition();
			break;

		case _2am_KEY_T:
			mE_fxmainRT.incrZPosition();
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