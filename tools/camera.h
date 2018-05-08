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

#include<windows.h>
class YP_Camera
{
	float yaw, pitch;
	float m_x_cc;
	float m_y_cc;
	int   window_width;
	int   window_height;
	bool  mousein = false;
public:
	YP_Camera() {}
	~YP_Camera() {}
	YP_Camera(int w, int h) :
		yaw(0),
		pitch(0),
		m_x_cc(0),
		m_y_cc(0),
		window_width(w), 
		window_height(h)
	{}

	inline float getYaw() { return yaw; }
	inline float getPitch() { return pitch; }
	
	//need position of cursor from windowhandler
	void cam_control(float movevel, float mousevel, bool mi, bool *wasd, glm::vec3& f_position)
	{
	 	if (mi)
		{
			POINT mouse;
			GetCursorPos(&mouse);
			//hide the cursor when in release

			m_x_cc = (float) mouse.x;
			m_y_cc = (float) mouse.y;
			//if (!yawyy) //lock mouse in scene-mode
			{
				yaw += mousevel*((window_width / 2) - m_x_cc);
				pitch += mousevel*((window_height / 2) - m_y_cc);
			}
			//lockcamera();

			SetCursorPos((int)window_width / 2, (int)window_height / 2);

			m_x_cc=(float)window_width/2;
			m_y_cc=(float)window_height/2;

			if (wasd[0])
			{
				if (pitch != 90.0f && pitch != -90.0f)
					move_camXZ(movevel, 0, f_position);
				move_camY(movevel, 0, f_position);
			}
			else if (wasd[2])
			{
				if (pitch != 90.0f && pitch != -90.0f)
					move_camXZ(movevel, 180.0f, f_position);
				move_camY(movevel, 180.0f, f_position);
			}
			else if (wasd[1])
				move_camXZ(movevel, 90.0f, f_position);
			else if (wasd[3])
				move_camXZ(movevel, 270.0f, f_position);
		}
	}

	void move_camXZ(float dist, float dir, glm::vec3 &f_position)
	{
		float rad = (yaw + dir) * M_PI / 180.0f;
		f_position.x -= sin(rad)*dist;
		f_position.z -= cos(rad)*dist;
	}

	void move_camY(float dist, float dir, glm::vec3 &f_position)
	{
		float rad = (pitch + dir) * M_PI / 180.0f;
		f_position.y += sin(rad)*dist;
	}

	void lockcamera()
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
};

#endif