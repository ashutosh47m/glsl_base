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
#include <windows.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <GLFW/glfw3.h>

class YP_Camera
{
	glm::vec3 up;
	glm::vec3 previousPosition;
	glm::vec3 right;
	glm::vec3 direction;
	POINT mouse;

	// position
	glm::vec3	position = glm::vec3(0, 20, 12);
	// horizontal angle : toward -Z, pitch
	float		horizontalAngle		= 3.14f;

	// vertical angle : 0, look at the horizon, yaw
	float		verticalAngle		= 0.0f;
	
	// Initial Field of View, can be used for zooming later.
	//float		initialFoV			= 45.0f;

	const float speed				= 8.3f; // units / second
	float		mouseSpeed;
	bool		enableFastCam			;	// allows you to move fast in the scene
	int			window_width;
	int			window_height;
	float		m_x_cc, m_y_cc;
	double		lastTime;
	double		currentTime;
	float		deltaTime;
public:

	glm::vec3 getPosition() 
	{
		return position;
	}

	YP_Camera() 
	{
		lastTime = glfwGetTime();
		enableFastCam = false;
		mouseSpeed = 0.005f;
	}
	~YP_Camera() {}
	YP_Camera(int w, int h) : window_width(w), window_height(h) 
	{
		lastTime = glfwGetTime();
		enableFastCam = false;
		mouseSpeed = 0.005f;
	}

	void cam_control(bool *wasd, glm::vec3& f_position, glm::mat4& view, glm::vec3& lookatdir);

	void setFastCam(bool val) { enableFastCam = val; }
	bool getFastCam() { return enableFastCam; }
};


class YP_Camera_debug_from_glsl_terrain
{
	int			window_width;
	int			window_height;
	float		m_x_cc, m_y_cc;
	YP_Camera_debug_from_glsl_terrain(int w, int h)
	{
		window_width = w;
		window_height = h;
		f_xPosition = 0;
		f_yPosition = 0;
		f_zPosition = -4;
		yaw = 0;
		pitch = 0;
	}

	float f_xPosition, f_yPosition, f_zPosition;
	float yaw, pitch;
	void move_camXZ(float dist, float dir);
	void move_camY(float dist, float dir);
	void lockcamera();
	void cam_controlYP(bool *wasd, glm::mat4&);

};
#endif