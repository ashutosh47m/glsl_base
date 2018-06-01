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

#if _DEBUG
	if(debug_cam)
		deltaTime = 1.0f;
#endif

	GetCursorPos(&mouse);
	SetCursorPos((int)window_width / 2, (int)window_height / 2);

	m_x_cc = (float)mouse.x;
	m_y_cc = (float)mouse.y;

	horizontalAngle += mouseSpeed * float(window_width  / 2 - m_x_cc);
	verticalAngle   += mouseSpeed * float(window_height / 2 - m_y_cc);


	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3
		(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	right = glm::vec3
		(
		sin(horizontalAngle - M_PI / 2.0f),
		0,
		cos(horizontalAngle - M_PI / 2.0f)
		);

	// Up vector : perpendicular to both direction and right
	up = glm::cross(right, direction);
	previousPosition = position;

	if (wasd[0])	//W
		position += direction * deltaTime * speed;
	if (wasd[2])	//S
		position -= direction * deltaTime * speed;
	if (wasd[3])	//D
		position += right	  * deltaTime * speed;
	if (wasd[1])	//A	
		position -= right	  * deltaTime * speed;
	//float distancee = glm::distance(previousPosition, position);

	view = glm::lookAt
		(
		position,				// Camera is here
		position + direction,	// and looks here : at the same position, plus "direction"
		up						// Head is up (set to 0,-1,0 to look upside-down)
		);

	//view = glm::lookAt ( position,	glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	lookatdir = position + direction;
	f_position = position;
}


void YP_Camera_debug_from_glsl_terrain::cam_controlYP(bool *wasd, glm::mat4& model)
{
	float movevel = .004f; 
	float mousevel = .4f; 
	bool mi = true;;
	if (mi)
	{
		POINT mouse;
		GetCursorPos(&mouse);
		m_x_cc = (float)mouse.x;
		m_y_cc = (float)mouse.y;

		{
			yaw += mousevel*((window_width / 2) - m_x_cc);
			pitch += mousevel*((window_height / 2) - m_y_cc);
		}
		lockcamera();

		SetCursorPos((int)window_width / 2, (int)window_height / 2);

		if (wasd[0])
		{
			if (pitch != 90.0f && pitch != -90.0f)
				move_camXZ(movevel, 0);
			move_camY(movevel, 0);
		}
		else if (wasd[2])
		{
			if (pitch != 90.0f && pitch != -90.0f)
				move_camXZ(movevel, 180.0f);
			move_camY(movevel, 180.0f);
		}
		else if (wasd[1])
			move_camXZ(movevel, 90.0f);
		else if (wasd[3])
			move_camXZ(movevel, 270.0f);
		/*		else if(q_k)
		window.move_camY(movevel,0);
		else if(z_k)
		window.move_camY(movevel,180);
		*/
	}

	model = glm::rotate(glm::mat4(1.0f), -pitch, glm::vec3(1, 0, 0))
		*   glm::rotate(glm::mat4(1.0f), -yaw, glm::vec3(0, 1, 0))
		*	glm::translate(glm::mat4(1.0f), glm::vec3(-f_xPosition, -f_yPosition, -f_zPosition));
}

void YP_Camera_debug_from_glsl_terrain::move_camXZ(float dist, float dir)
{
	float rad = (yaw + dir) * M_PI / 180.0f;
	f_xPosition -= sin(rad)*dist;
	f_zPosition -= cos(rad)*dist;
}

void YP_Camera_debug_from_glsl_terrain::move_camY(float dist, float dir)
{
	float rad = (pitch + dir) * M_PI / 180.0f;
	f_yPosition += sin(rad)*dist;
}

void YP_Camera_debug_from_glsl_terrain::lockcamera()
{
	//do not move it out of bounds.
	if (pitch>90)
		pitch = 90.0f;
	else if (pitch<-45)
		pitch = -45.0f;

	if (yaw<0)
		yaw += 360.0f;
	else if (yaw>360)
		yaw -= 360.0f;
}
