/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

#include "shader_program.h"

std::string Shader::loadFromFile(std::string s)
{
	//_getcwd();
	ifstream inFile(s.c_str());
	if (!inFile)
	{
		swprintf(ws, 1000, L"Error opening file: %hs\n", s.c_str());
		printf("%s\n", s.c_str());
		//MessageBox(NULL, ws, NULL, NULL);
		printf("Error opening file: %s\n", s.c_str());
	}
	std::stringstream fileBuf;
	fileBuf << inFile.rdbuf();
	inFile.close();
	std::string code(fileBuf.str());
	return code;
}

void Shader::deleteShader()
{
	glDeleteShader(shaderHandle);
}

GLuint Shader::createShader(GLuint s)
{
	// Create the shader object
	GLuint shaderhandle = glCreateShader(s);
	if (0 == shaderhandle)
	{
		//swprintf(ws, 1000, L"Error creating shader.\n" );
		//		MessageBox(NULL, ws,NULL, NULL);
		//	fprintf(stderr, "Error creating shader.\n");
	}
	return shaderhandle;
}

bool Shader::compileShader(GLuint shaderhandle)
{
	// Compile the shader
	glCompileShader(shaderhandle);
	// Check shader compilation for error
	GLint result;
	glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &result);

	if (GL_FALSE == result)
	{
		//swprintf(ws, 1000, L"shaderhandle compilation failed\n" );
		//MessageBox(NULL, ws,NULL, NULL);
		// fprintf( stderr, "shaderhandle compilation failed!\n" );
		GLint logLen;
		glGetShaderiv(shaderhandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shaderhandle, logLen, &written, log);
			swprintf(ws, 1000, L"Shader compilation failed\n\n%hs", log);
			printf("%s\n", log);
			//MessageBox(NULL, ws, NULL, NULL);
			printf("log: \n%s", log);
			free(log);
		}
		return false;
	}
	else return true;
}

void Shader::initializeShader(std::string sourcefile, int shadertype)
{
	std::string code;
	//type = shadertype;

	switch (shadertype)
	{
	case VERTEX_SHADER:
		sourcefile.append(".vs");
		code = Shader::loadFromFile(sourcefile);
		shaderHandle = Shader::createShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT_SHADER:
		sourcefile.append(".fs");
		code = Shader::loadFromFile(sourcefile);
		shaderHandle = Shader::createShader(GL_FRAGMENT_SHADER);
		break;
	}
	const char *source = code.c_str();
	glShaderSource(shaderHandle, 1, &source, NULL);

	if (Shader::compileShader(shaderHandle) == false)
		exit(-9);
}


// --------------- ------------- ---------------------// --------------- ------------- ---------------------// --------------- ------------- ---------------------
// --------------- ShaderProgram ---------------------// --------------- ShaderProgram ---------------------// --------------- ShaderProgram ---------------------
// --------------- ------------- ---------------------// --------------- ------------- ---------------------// --------------- ------------- ---------------------

void ShaderProgram::createProgram(std::string sourcefile)
{
	programHandle = glCreateProgram();
	if (programHandle == 0)
		printf("error creating program\n");
		//MessageBox(NULL, L"Error creating program\n", NULL, NULL);

	// initialize vertex shader 
	shaderVS.initializeShader(sourcefile.c_str(), E_Shaders::VERTEX_SHADER);
	glAttachShader(programHandle, shaderVS.getShaderHandle());

	// initialize fragment shader 
	shaderFS.initializeShader(sourcefile.c_str(), E_Shaders::FRAGMENT_SHADER);
	glAttachShader(programHandle, shaderFS.getShaderHandle());

	// link the two shaders
	if (!link(programHandle))
	{
		printf("error while linking shader\n");
	}
}

ShaderProgram::~ShaderProgram() //shutdown shaders here
{
	glDetachShader(programHandle, shaderVS.getShaderHandle());
	glDetachShader(programHandle, shaderVS.getShaderHandle());
	shaderVS.deleteShader();
	shaderFS.deleteShader();
	glDeleteProgram(programHandle);
}

ShaderProgram::ShaderProgram()
{
	programHandle = NULL;
	printf("shaderprogram constructor\n");
}

// you can either call a constructor with shader path or call initShader with shaderpath
ShaderProgram::ShaderProgram(std::string path) : shaderPath(path) 
{
	createProgram(path);
}

bool ShaderProgram::link(GLuint handle)
{
	glLinkProgram(handle);

	//check for linking errors 
	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		GLint logLen;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(handle, logLen, &written, log);
			printf("%s\n", log);
			//MessageBox(NULL, ws, NULL, NULL);
			//fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
		return false;
	}
	else
		return true;
}


GLuint ShaderProgram::getShaderProgramHandle()
{
	return programHandle;
}

void ShaderProgram::setUniform(const char *name, float x, float y, float z)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniform3f(loc, x, y, z);
	}
	else
	{
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);

		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, const glm::vec3 &v)
{
	//		glUseProgram(handle);

	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniform3f(loc, v.x, v.y, v.z);
	}
	else
	{
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, const glm::vec4 &v)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}
	else
	{
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, const glm::vec2 & v)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0) {
		glUniform2f(loc, v.x, v.y);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, const glm::mat4 &m)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, const glm::mat3 &m)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, float val)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniform1f(loc, val);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//	MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, int val)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{

		glUniform1i(loc, val);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, unsigned int val)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniform1i(loc, val);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//	//MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}

void  ShaderProgram::setUniform(const char *name, bool val)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(programHandle, name);
	if (loc >= 0)
	{
		glUniform1i(loc, val);
	}
	else {
		//	swprintf(ws, 1000, L"Uniform var %d not found", name);
		//	MessageBox(NULL, ws,NULL, NULL);
		//	printf("Uniform: %s not found.\n",name);
	}
}