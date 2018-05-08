#include "testScene.h"

using namespace usr_scene;

void TestScene::initAxes()
{
	std::vector<float> pos_line = { -100,0,0,
		100,0,0 };

	std::vector<float> col_line = { 1, 1, 1,
		1, 1, 1 };
	mE_X_axes.initEntity(pos_line, col_line);
	pos_line = { 0,-100,0,
		0,100,0 };
	mE_Y_axes.initEntity(pos_line, col_line);
	pos_line = { 0,0,100,
		0,0,-100 };
	mE_Z_axes.initEntity(pos_line, col_line);
}

void TestScene::initialize()
{
	loadShaders();

	// triangles
	mE_triangle.initEntity();	//inits the shaders and other resources
	mE_red_triangle.initEntity();
	mE_texturedTriangle.initEntity(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");

	// inititlize the axes
	initAxes();

	//quad
	mE_quad.initEntity();
	mE_Woodenquad.initEntity(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");
	mE_Marblequad.initEntity(++globalTextureCount, "..\\resources\\textures\\marble.jpg");
	mE_grassStonequad.initEntity(++globalTextureCount, "..\\resources\\textures\\grassstone.jpg");
}

void TestScene::update()
{
	/*
	wasd[0] = WINDOW_WIN32::w;
	wasd[1] = WINDOW_WIN32::a;
	wasd[2] = WINDOW_WIN32::s;
	wasd[3] = WINDOW_WIN32::d;
	if (WINDOW_WIN32::w || WINDOW_WIN32::a || WINDOW_WIN32::s || WINDOW_WIN32::d)
	{
	printf("halchal\n");
	}
	*/
	/*t_camera.cam_control(
	.3f, // move velocity
	.3f, // mouse velocity
	true,
	wasd, // a list of bools containing which of the WASD is pressed.
	data.f_position
	);

	printf("%f %f  \n", -t_camera.getYaw(), -t_camera.getPitch());
	// update everything here.
	data.glm_model =  	glm::rotate   ( glm::mat4(1.0f), -t_camera.getPitch(), glm::vec3(1, 0, 0))
	*   glm::rotate   ( glm::mat4(1.0f), -t_camera.getYaw(), glm::vec3(0, 1, 0))
	*	glm::translate( glm::mat4(1.0f), glm::vec3(-data.f_position.x, -data.f_position.y, -data.f_position.z));
	*/
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

	glBindVertexArray(0);
}



void TestScene::keyProcess(int key, int scancode, int action, int mods)
{
	if (action == _2am_KEY_PRESS)
	{
		switch (key)
		{
		case _2am_KEY_W:
			printf("true\n");
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

		case _2am_KEY_UP:
			camera_z++;
			break;
		case _2am_KEY_DOWN:
			camera_z--;
			break;
		case _2am_KEY_RIGHT:
			camera_x++;
			break;
		case _2am_KEY_LEFT:
			camera_x--;
			break;
		}
	}

	else if (action == _2am_KEY_RELEASE)
	{
		switch (key)
		{
		case _2am_KEY_W:
			wasd[0] = false;
			printf("false\n");
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
}
