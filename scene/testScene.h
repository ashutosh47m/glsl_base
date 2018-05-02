/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef TESTSCN
#define TESTSCN

#include "scene_handler.h"

#include "../Entity/E_origin_axis.h"
#include "../Entity/E_first_triangle.h"
#include "../Entity/E_first_red_triangle.h"
#include "../Entity/E_colored_quad.h"
#include "../Entity/E_textured_quad.h"
#include "../Entity/E_textured_triangle.h"

#include "../resources/texture_handler.h"
#include "../tools/camera.h"

namespace usr_scene
{

	class TestScene : public AbstractScene
	{
		E_first_triangle		e_triangle;			// e stands for entity
		E_textured_triangle		e_texturedTriangle;
		E_first_red_triangle	e_red_triangle;
		E_colored_quad			e_quad;
		E_textured_quad			e_Woodenquad;
		E_textured_quad			e_Marblequad;
		E_textured_quad			e_grassStonequad;
		E_colored_line			e_X_axes, e_Y_axes, e_Z_axes;
		
		YP_Camera				t_camera;			// t stands for tools, i.e. YP_Camera comes under tools category

		bool					wasd[4] = { false };

		void initialize()
		{
			loadShaders();
			// set the camera here.
			// since the camera is a part of the scene we will be initializing it here.
			data.glm_view = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(00, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

			// the global camera matrix, represents where the camera is in the scene 
			data.glm_model = glm::mat4(1.0f);

			// triangled
			e_triangle.initEntity();	//inits the shaders and other resources
			e_red_triangle.initEntity();
			e_texturedTriangle.initEntity(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");

			std::vector<float> pos_line = { -100,0,0,
										100,0,0  };
			
			std::vector<float> col_line  = { 1, 1, 1,
										1, 1, 1 };
			e_X_axes.initEntity(pos_line, col_line);
			pos_line = { 0,-100,0,
						0,100,0 };
			e_Y_axes.initEntity(pos_line, col_line);
			pos_line = { 0,0,100,
						0,0,-100 };
			e_Z_axes.initEntity(pos_line, col_line);

			//quad
			e_quad.initEntity();
			e_Woodenquad.initEntity(++globalTextureCount, "..\\resources\\textures\\wooden.jpg");
			e_Marblequad.initEntity(++globalTextureCount, "..\\resources\\textures\\marble.jpg");
			e_grassStonequad.initEntity(++globalTextureCount, "..\\resources\\textures\\grassstone.jpg");
		}

		void update()
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
			//t_camera.cam_control(
			//				.3f, // move velocity
			//				.3f, // mouse velocity
			//				true,
			//				wasd, // a list of bools containing which of the WASD is pressed.
			//				data.f_position
			//	);

	//		printf("%f %f  \n", -t_camera.getYaw(), -t_camera.getPitch());
			// update everything here.
	//		data.glm_model =  	glm::rotate   ( glm::mat4(1.0f), -t_camera.getPitch(), glm::vec3(1, 0, 0))
	//						*   glm::rotate   ( glm::mat4(1.0f), -t_camera.getYaw(), glm::vec3(0, 1, 0))
	//						*	glm::translate( glm::mat4(1.0f), glm::vec3(-data.f_position.x, -data.f_position.y, -data.f_position.z));
		}
		void draw()
		{
			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

			// binding triangles vertex array here because we are using a single buffers for both the triangls
			glBindVertexArray(e_triangle.getVAOHandle());
			e_triangle.draw(data, getShaderLibrary()->colored_geometry);
			e_red_triangle.draw(data, getShaderLibrary()->red_triangle_shader);
		
			glBindVertexArray(e_texturedTriangle.getVAOHandle());
			e_texturedTriangle.draw(data, glm::vec3(2, 1, 0), getShaderLibrary()->textured_geometry);

			glBindVertexArray(e_quad.getVAOHandle());
			e_quad.draw(data, getShaderLibrary()->colored_geometry);

			glBindVertexArray(e_Woodenquad.getVAOHandle());
			e_Woodenquad.draw(data, glm::vec3(-1, 1, 0), getShaderLibrary()->textured_geometry);
			e_Woodenquad.draw(data, glm::vec3(-3, 1, 0), getShaderLibrary()->textured_geometry);
			e_Marblequad.draw(data, glm::vec3(-1, 2, 0), getShaderLibrary()->textured_geometry);
			e_grassStonequad.draw(data, glm::vec3(1, 2, 0), getShaderLibrary()->textured_geometry);

			glBindVertexArray(e_X_axes.getVAOHandle());
			e_X_axes.draw(data, getShaderLibrary()->colored_geometry);
			glBindVertexArray(e_Y_axes.getVAOHandle());
			e_Y_axes.draw(data, getShaderLibrary()->colored_geometry);
			glBindVertexArray(e_Z_axes.getVAOHandle());
			e_Z_axes.draw(data, getShaderLibrary()->colored_geometry);

			glBindVertexArray(0);
		}

	public:
		TestScene()
		{

		}
		~TestScene()
		{
		}
	};
};

#endif
