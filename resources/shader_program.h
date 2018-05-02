/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/GL.h>

using std::ifstream;

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

enum E_Shaders 
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

class Shader // shader is not the resource here .... it is shader program that comes under resource..
{
	GLuint		shaderHandle;

	wchar_t		ws[1000]; //for error logs

	std::string loadFromFile(std::string);
	GLuint		createShader(GLuint);
	bool		compileShader(GLuint);

public:
	GLuint		getShaderHandle() { return shaderHandle; }

	void		initializeShader(std::string, int);
	void		deleteShader(); // this shall be called by the program in which the shader belongs
};

//	this class has tools to load, create, and maintain shaders
class ShaderProgram 
{
	GLuint		programHandle;
	int			type;
	Shader		shaderVS, shaderFS;
public:
	ShaderProgram();
	ShaderProgram(std::string);
	~ShaderProgram();

	std::string shaderPath;

	wchar_t		ws[1000]; //for error logs

	//	path of shader, 
	//	no need to supply VS and FS, the program will automatically load all the shaders associated with a particular shader program
	
	void		createProgram(std::string);
	bool		link(GLuint);

	GLuint		getShaderProgramHandle();
	//void		setShaderProgramHandle(GLuint);

	void		setUniform(const char *name, float x, float y, float z);
	void		setUniform(const char *name, const glm::vec2 & v);
	void		setUniform(const char *name, const glm::vec3 & v);
	void		setUniform(const char *name, const glm::vec4 & v);
	void		setUniform(const char *name, const glm::mat4 & m);
	void		setUniform(const char *name, const glm::mat3 & m);
	void		setUniform(const char *name, float val);
	void		setUniform(const char *name, unsigned int val);
	void		setUniform(const char *name, int val);
	void		setUniform(const char *name, bool val);
};

class ShaderLibrary
{
	//this class load all the shaders needed to render the objects.
	//here we make sure that the shaders are loaded only once

public:

	ShaderProgram *red_triangle_shader;	//first_red_triangle
	ShaderProgram *triangle_shader;		//first_triangle
	ShaderProgram *textured_geometry;	//textured_geometry
	ShaderProgram *colored_geometry;	//colored_geometry

	ShaderLibrary()
	{ }

	void loadShaders()
	{
		red_triangle_shader = new ShaderProgram("../src/renderer/shaders/first_red_triangle");
		triangle_shader = new ShaderProgram("../src/renderer/shaders/first_triangle");
		textured_geometry = new ShaderProgram("../src/renderer/shaders/textured_geometry");
		colored_geometry = new ShaderProgram("../src/renderer/shaders/colored_geometry");
	}

	~ShaderLibrary()
	{
		if (red_triangle_shader != NULL)
			delete red_triangle_shader;
		if (triangle_shader != NULL)
			delete triangle_shader;
		if (textured_geometry != NULL)
			delete textured_geometry;
		if (colored_geometry != NULL)
			delete colored_geometry;
	}
};

#endif