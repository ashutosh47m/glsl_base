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
public:
	GLuint		uniform_ID;
	GLuint		ui_texID;
	std::string	uniform_var;
	std::string	location;
	
	GLuint getTextureID() { return ui_texID; }
	GLuint getUniformID() { return uniform_ID; }
};

class texture_jpg : public texture
{
	GLuint jpg_ID;
public:
	
	texture_jpg(GLuint& cnt, std::string u_var, std::string text_location)
	{
		uniform_var		= u_var;
		location		= text_location;
		uniform_ID		= ++cnt;
		loadtexture();
	}

	texture_jpg(){}
	~texture_jpg()
	{
		glDeleteTextures(1, &ui_texID);
		ilDeleteImage(jpg_ID);
	}

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
				printf("image conversion failed\n");
				//MessageBox(NULL, L"image conversion failed", NULL, NULL);
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
			printf(" %ls texture not loaded\n", t.c_str());
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

class texture3D
{
	int			XDIM;
	int			YDIM;
	int			ZDIM;

	GLuint		uniform_ID;
	GLuint		ui_texID;
	std::string	uniform_var;
	bool		isValid = false;
	
public: 
	texture3D()  {}
	~texture3D() {}

	bool isVolumeFileValid() { return isValid; }

	texture3D(GLuint textureID, std::string volume_file, std::string u_var,
		int xdim, int ydim, int zdim) :
		uniform_ID(textureID), 
		uniform_var(u_var),
		XDIM(xdim), YDIM(ydim), ZDIM(zdim)
	{
		std::ifstream infile(volume_file.c_str(), std::ios_base::binary);

		if (infile.good())
		{
			//read the volume data file
			GLubyte* pData = new GLubyte[XDIM*YDIM*ZDIM];
			infile.read(reinterpret_cast<char*>(pData), XDIM*YDIM*ZDIM*sizeof(GLubyte));
			infile.close();

			//generate OpenGL texture
			glGenTextures(1, &ui_texID);
			glActiveTexture(GL_TEXTURE0 + uniform_ID);
			glBindTexture(GL_TEXTURE_3D, ui_texID);

			// set the texture parameters
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			//set the mipmap levels (base and max)
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);

			//allocate data with internal format and foramt as (GL_RED)		
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, XDIM, YDIM, ZDIM, 0, GL_RED, GL_UNSIGNED_BYTE, pData);
			// should use this often #define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);

				//generate mipmaps
				glGenerateMipmap(GL_TEXTURE_3D);

			//delete the volume data allocated on heap
			delete[] pData;
			isValid = true;
			glBindTexture(GL_TEXTURE_3D, 0);
		}
		else 
		{
			printf("the volume file supplied is not supported\n");
			isValid = false;
		}
	}

	const char *getUniformVar() { return uniform_var.c_str(); }
	GLuint getUniformID() { return uniform_ID; }
	GLuint getTextureID() { return ui_texID; }

	void deleteResource()
	{
		glDeleteTextures(1, &ui_texID);
	}
};

#endif