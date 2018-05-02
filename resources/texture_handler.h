/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

// loads images to be used in  OpenGL.

#ifndef _TEXT_H
#define _TEXT_H

#include "IL/il.h"
#include "shader_program.h"

class texture
{
	GLuint		width;                              
	GLuint		height;                             
	GLuint		uniform_ID;  
	GLuint		ui_texID;
	GLuint		jpg_ID;
	std::string	uniform_var;
	std::string	location;

public:
	GLuint getTextureID() { return ui_texID; }
	GLuint getUniformID() { return uniform_ID; }
	texture(GLuint cnt, std::string u_var, std::string text_location) :
		uniform_ID(cnt),
		uniform_var(u_var),
		location(text_location)
	{
		loadtexture();
	}
	texture(){}
	~texture() 
	{
	}

	void deleteResource()
	{
		glDeleteTextures(1, &ui_texID);
		ilDeleteImage(jpg_ID);
	}

	//void init()
	//{
	//	loadtexture();
	//}
	bool loadtexture()
	{
		float aniso = 16.0f;
		std::wstring t;
		for (unsigned int i = 0; i < location.length(); ++i)
			t += wchar_t(location[i]);

		//text.format = JPG;
		ILboolean success;
		ilInit();
		ilGenImages(1, &jpg_ID);
		ilBindImage(jpg_ID);

		success = ilLoadImage(t.c_str());
		if (success)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
																unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
			if (!success)
			{
				MessageBox(NULL, L"image conversion failed", NULL, NULL);
			}
			glGenTextures(1, &ui_texID);
			glActiveTexture(GL_TEXTURE0 + uniform_ID);
			glBindTexture(GL_TEXTURE_2D, ui_texID);

			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

			glTexImage2D
			(
				GL_TEXTURE_2D,
				0,
				ilGetInteger(IL_IMAGE_BPP),
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				GL_UNSIGNED_BYTE,
				ilGetData()
			);
		}
		else
		{
			MessageBox(NULL, L"Texture not loaded", NULL, NULL);
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// Linear Filtered
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// Linear Filtered
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // Linear Filtered
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // Linear Filtered
		return true;
	}
};

#endif