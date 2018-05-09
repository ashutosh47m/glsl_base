/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#ifndef _CAMERA
#define _CAMERA


#ifndef M_PI
#define M_PI 3.1415926535f
#endif
#include <GLFW/glfw3.h>
#include<windows.h>
class YP_Camera
{
	// position
	glm::vec3 position = glm::vec3(0, 0, 1);
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 0.0003f; // 0.003f units / second
	float mouseSpeed = 0.005f;

	int   window_width;
	int   window_height;
	float m_x_cc, m_y_cc;

public:
	YP_Camera() {}
	~YP_Camera() {}
	YP_Camera(int w, int h) : window_width(w), window_height(h) {}

	//need position of cursor from windowhandler
	void cam_control(bool *wasd, glm::vec3& f_position, glm::mat4& view)
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();

		float deltaTime = float(currentTime - lastTime);

		POINT mouse;
		GetCursorPos(&mouse);
		//hide the cursor when in release

		m_x_cc = (float)mouse.x;
		m_y_cc = (float)mouse.y;

		horizontalAngle += mouseSpeed * float(window_width / 2 - m_x_cc);
		verticalAngle += mouseSpeed * float(window_height / 2 - m_y_cc);
		
		SetCursorPos((int)window_width / 2, (int)window_height / 2);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - M_PI / 2.0f),
			0,
			cos(horizontalAngle - M_PI / 2.0f)
			);

		// Up vector : perpendicular to both direction and right
		glm::vec3 up = glm::cross(right, direction);
		
		if (wasd[0])
			position += direction * deltaTime* speed;
		if (wasd[2])
			position -= direction * deltaTime*speed;
		if (wasd[3])
			position += right * deltaTime*speed;
		if (wasd[1])
			position -= right * deltaTime*speed;

		//printf("%f %f %f \n", position.x, position.y, position.z );

		view = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
			);

		f_position = position;
	}
};

#endif