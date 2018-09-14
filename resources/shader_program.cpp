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
		printf("shader compilation failed! \n");
}


// --------------- ------------- ---------------------// --------------- ------------- ---------------------// --------------- ------------- ---------------------
// --------------- ShaderProgram ---------------------// --------------- ShaderProgram ---------------------// --------------- ShaderProgram ---------------------
// --------------- ------------- ---------------------// --------------- ------------- ---------------------// --------------- ------------- ---------------------

void ShaderProgram::createProgram(std::string sourcefile)
{
	m_ProgramHandle = glCreateProgram();
	if (m_ProgramHandle == 0)
		printf("error creating program\n");
		//MessageBox(NULL, L"Error creating program\n", NULL, NULL);

	// initialize vertex shader 
	m_ShaderVS.initializeShader(sourcefile.c_str(), E_Shaders::VERTEX_SHADER);
	glAttachShader(m_ProgramHandle, m_ShaderVS.getShaderHandle());

	// initialize fragment shader 
	m_ShaderFS.initializeShader(sourcefile.c_str(), E_Shaders::FRAGMENT_SHADER);
	glAttachShader(m_ProgramHandle, m_ShaderFS.getShaderHandle());

	// link the two shaders
	if (!link(m_ProgramHandle))
	{
		printf("error while linking shader\n");
	}
}

ShaderProgram::~ShaderProgram() //shutdown shaders here
{
	glDetachShader(m_ProgramHandle, m_ShaderVS.getShaderHandle());
	glDetachShader(m_ProgramHandle, m_ShaderVS.getShaderHandle());
	m_ShaderVS.deleteShader();
	m_ShaderFS.deleteShader();
	glDeleteProgram(m_ProgramHandle);
}

ShaderProgram::ShaderProgram()
{
	m_ProgramHandle = NULL;
	printf("shaderprogram constructor\n");
}

// you can either call a constructor with shader path or call initShader with shaderpath
ShaderProgram::ShaderProgram(std::string path, std::string name) : 
	m_ShaderPath(path), 
	m_ShaderName(name)
{
	createProgram(m_ShaderPath);
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
	return m_ProgramHandle;
}

void ShaderProgram::setUniform(const char *name, float x, float y, float z)
{
	//		glUseProgram(handle);
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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

	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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
	int loc = glGetUniformLocation(m_ProgramHandle, name);
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