/*
GLSL_base
May 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _CAMERA
#define _CAMERA


#ifndef M_PI
#define M_PI 3.1415926535f
#endif
#include <GLFW/glfw3.h>
#include <windows.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

class YP_Camera
{
	glm::vec3 up;
	glm::vec3 previousPosition;
	glm::vec3 right;
	glm::vec3 direction;
	POINT mouse;

	// position
	glm::vec3	position = glm::vec3(0, 0, 1);
	// horizontal angle : toward -Z, pitch
	float		horizontalAngle		= 3.14f;

	// vertical angle : 0, look at the horizon, yaw
	float		verticalAngle		= 0.0f;
	
	// Initial Field of View, can be used for zooming later.
	//float		initialFoV			= 45.0f;

	const float speed				= 3.3f; // units / second
	float		mouseSpeed			= 0.005f;

	int			window_width;
	int			window_height;
	float		m_x_cc, m_y_cc;
	double		lastTime;
	double		currentTime;
	float		deltaTime;
public:
	YP_Camera() : lastTime(glfwGetTime()) { }
	~YP_Camera() {}
	YP_Camera(int w, int h) : window_width(w), window_height(h) {}
	void cam_control(bool *wasd, glm::vec3& f_position, glm::mat4& view, glm::vec3& lookatdir);
};

#endif