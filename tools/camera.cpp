/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#include "camera.h"

void YP_Camera::cam_control(bool *wasd, glm::vec3& f_position, glm::mat4& view, glm::vec3& lookatdir)
{
	// Compute time difference between current and last frame
	lastTime    = currentTime;
	currentTime = glfwGetTime();
	deltaTime   = float(currentTime - lastTime);

	POINT mouse;
	GetCursorPos(&mouse);
	SetCursorPos((int)window_width / 2, (int)window_height / 2);

	m_x_cc = (float)mouse.x;
	m_y_cc = (float)mouse.y;

	horizontalAngle += mouseSpeed * float(window_width  / 2 - m_x_cc);
	verticalAngle   += mouseSpeed * float(window_height / 2 - m_y_cc);


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction
		(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3
		(
		sin(horizontalAngle - M_PI / 2.0f),
		0,
		cos(horizontalAngle - M_PI / 2.0f)
		);

	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross(right, direction);
	glm::vec3 previousPosition = position;

	if (wasd[0])	//W
		position += direction * deltaTime * speed;
	if (wasd[2])	//S
		position -= direction * deltaTime * speed;
	if (wasd[3])	//D
		position += right	  * deltaTime * speed;
	if (wasd[1])	//A	
		position -= right	  * deltaTime * speed;
	float distancee = glm::distance(previousPosition, position);
	printf("delta %f \n", distancee);

	view = glm::lookAt
		(
		position,				// Camera is here
		position + direction,	// and looks here : at the same position, plus "direction"
		up						// Head is up (set to 0,-1,0 to look upside-down)
		);

	lookatdir = position + direction;
	f_position = position;
}
