/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#include <fstream>
#include <sstream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/GL.h>
#include <vector>

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
	std::string m_ShaderName;
	GLuint		m_ProgramHandle;
	int			m_Type;
	Shader		m_ShaderVS, m_ShaderFS;
public:
	ShaderProgram();
	ShaderProgram(std::string, std::string);
	~ShaderProgram();

	std::string m_ShaderPath;

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

	ShaderProgram *red_triangle_shader;			//first_red_triangle
	ShaderProgram *triangle_shader;				//first_triangle
	ShaderProgram *textured_colored_geometry;	//textured_geometry
	ShaderProgram *sun;							//sun
	ShaderProgram *volume_shader;				//volume: 3d texture
	ShaderProgram *volume_ray_caster;			//volume: ray caster
	ShaderProgram *mesh;						//meshloader : generic
	ShaderProgram *terrain;						//meshloader : terrain
	// post fx shaders
	ShaderProgram *fx_rendertarget;				// a simple quad used for rendertargets
	ShaderProgram *fx_lightscatter;				// god rays
	ShaderProgram *fx_grayscale;
	ShaderProgram *fx_combineLightscatter;
	ShaderProgram *fx_deferred_ADS;

	ShaderLibrary()
	{ }

	void loadShaders()
	{
		std::string filename = "C:\\Programming\\OpenGL\\glsl_base";

		red_triangle_shader			= new ShaderProgram(filename+"/renderer/shaders/first_red_triangle",			"first_red_triangle");
		triangle_shader				= new ShaderProgram(filename+"/renderer/shaders/first_triangle",				"first_triangle");
		textured_colored_geometry	= new ShaderProgram(filename+"/renderer/shaders/textured_colored_geometry",	"textured_colored_geometry");
		sun							= new ShaderProgram(filename+"/renderer/shaders/sun",							"sun");
		volume_shader				= new ShaderProgram(filename+"/renderer/shaders/volume",						"volume");
		volume_ray_caster			= new ShaderProgram(filename+"/renderer/shaders/ray_caster",					"ray_caster");
		mesh						= new ShaderProgram(filename+"/renderer/shaders/flat_mesh",					"flat_mesh");
		terrain						= new ShaderProgram(filename+"/renderer/shaders/terrain",						"terrain");

		fx_rendertarget				= new ShaderProgram(filename+"/renderer/shaders/rendertarget",					"rendertarget");
		fx_lightscatter				= new ShaderProgram(filename+"/renderer/shaders/lightscatter",					"lightscatter");
		fx_grayscale				= new ShaderProgram(filename+"/renderer/shaders/grayscale",					"grayscale");
		fx_combineLightscatter		= new ShaderProgram(filename+"/renderer/shaders/combineLightscatter",			"combineLightscatter");
		fx_deferred_ADS				= new ShaderProgram(filename+"/renderer/shaders/def_ADS",						"def_ADS");
	}

	~ShaderLibrary()
	{
		if (fx_combineLightscatter != NULL)
			delete fx_combineLightscatter;
		if (fx_deferred_ADS != NULL)
			delete fx_deferred_ADS;
		if (fx_rendertarget != NULL)
			delete fx_rendertarget;
		if (fx_grayscale!= NULL)
			delete fx_grayscale;
		if (red_triangle_shader != NULL)
			delete red_triangle_shader;
		if (triangle_shader != NULL)
			delete triangle_shader;
		if (textured_colored_geometry != NULL)
			delete textured_colored_geometry;
		if (sun != NULL)
			delete sun;
		if (volume_shader != NULL)
			delete volume_shader;
		if (mesh != NULL)
			delete mesh;
		if (terrain != NULL)
			delete terrain;
		if (volume_ray_caster != NULL)
			delete volume_ray_caster;
		if (fx_lightscatter!= NULL)
			delete fx_lightscatter;
	}
};

#endif