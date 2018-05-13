#include "testScene.h"

using namespace usr_scene;

void TestScene::initAxes()
{
	std::vector<float> pos_line = { -100,0,0, 100,0,0 };
	std::vector<float> col_line = { 0,0,0,0,0,0};
	mE_X_axes.initEntity(pos_line, col_line);
	pos_line = { 0,-100,0, 0,100,0 };
	mE_Y_axes.initEntity(pos_line, col_line);
	pos_line = { 0,0,100, 0,0,-100 };
	mE_Z_axes.initEntity(pos_line, col_line);
}

void TestScene::initialize()
{
	loadShaders();

	// triangles
	mE_triangle.initEntity();	//inits the shaders and other resources
	mE_red_triangle.initEntity();
	mE_texturedTriangle.initEntity	(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");

	// inititlize the axes
	initAxes();

	//quad
	mE_quad.initEntity();
	mE_Woodenquad.initEntity		(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");
	mE_Marblequad.initEntity		(++globalTextureCount, "..\\resources\\textures\\marble.jpg");
	mE_grassStonequad.initEntity	(++globalTextureCount, "..\\resources\\textures\\grassstone.jpg");
	
	//volume
	mEvol.initEntity(++globalTextureCount, "..\\resources\\volumes\\engine256.raw");
	//load camera
	mT_camera = new YP_Camera(m_width, m_height);
}

void TestScene::update()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.55f, .515f, .515f, 1);

	mT_camera->cam_control(wasd, data.f_position, data.glm_view, mViewDirection);
}

void TestScene::draw()
{
	//enable function will call glBindVertexArray function for particular entity.
	// some entities are repeated, for them the call to glBindVertexArray is redundant.
	// for such entities, you will have to call glBindVertexArray explicitely.
	// please have a look at draw method inside entity for more information.

	mE_triangle.draw(data, getShaderLibrary()->colored_geometry);
	mE_red_triangle.draw(data, getShaderLibrary()->red_triangle_shader);

	mE_texturedTriangle.draw(data, glm::vec3(2, 1, 0), getShaderLibrary()->textured_geometry);

	mE_quad.draw(data, getShaderLibrary()->colored_geometry);

	mE_Woodenquad.enable();
	mE_Woodenquad.draw(data, glm::vec3(-1, 1, 0), getShaderLibrary()->textured_geometry);
	mE_Woodenquad.draw(data, glm::vec3(-3, 1, 0), getShaderLibrary()->textured_geometry);
	mE_Marblequad.draw(data, glm::vec3(-1, 2, 0), getShaderLibrary()->textured_geometry);
	mE_grassStonequad.draw(data, glm::vec3(1, 2, 0), getShaderLibrary()->textured_geometry);

	mE_X_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Y_axes.draw(data, getShaderLibrary()->colored_geometry);
	mE_Z_axes.draw(data, getShaderLibrary()->colored_geometry);

	mEvol.SliceVolume(mViewDirection);

	mEvol.draw(data, getShaderLibrary()->volume_shader);
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
		switch (key)
		{
		case _2am_KEY_W:
			camera_z++;
			break;
		case _2am_KEY_S:
			camera_z--;
			break;
		}
	}
}