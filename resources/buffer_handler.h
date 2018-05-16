/*
GLSL_base
Nov 2017, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/


/*
	Data sent through buffers can be used by any shaders in the engine.
	Then question arises, why should be declare shader_buffer var inside shader class?
		> because you've got to know which shader is using which buffer.
		> u SHOULD BE ABLE TO reuse buffer sent by one shader in other shader
*/
#include <vector>

#ifndef _SHADER_BUF
#define _SHADER_BUF
#define EPSILON 0.0001f

class ShaderBuffer
{
public:
	virtual GLuint 	getBufferHandle() { return 999; }
	virtual void 	initResource() = 0;
	virtual void 	deleteResource() = 0;
	virtual GLuint 	getVAOHandle() = 0;
};

class ShaderBuffer_3d_texture_volume : public ShaderBuffer
{
	GLuint bufferHandle;
	GLuint vaoHandle;

public:
	ShaderBuffer_3d_texture_volume() {}
	ShaderBuffer_3d_texture_volume(GLsizei sizeofslices)
	{
		initResource();

		glGenVertexArrays(1, &vaoHandle);
		glGenBuffers(1, &bufferHandle);

		glBindVertexArray(vaoHandle);
		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);

		//pass the sliced vertices vector to buffer object memory
		glBufferData(GL_ARRAY_BUFFER, sizeofslices, 0, GL_DYNAMIC_DRAW);

		//enable vertex attribute array for position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	GLuint getBufferHandle() { return bufferHandle; }

	void deleteResource()
	{
		glDeleteBuffers(1, &bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};

class ShaderBuffer_POS : public ShaderBuffer
{
	GLuint bufferHandle;
	GLuint vaoHandle;
	std::vector <GLfloat>data;
public:
	ShaderBuffer_POS()
	{
		initResource();
	}

	ShaderBuffer_POS(const std::initializer_list<float> &v)
	{
		initResource();
		for (auto itm : v)
		{
			data.push_back(itm);
		}

		glGenBuffers(1, &bufferHandle);
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);
			glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(GLfloat)*data.size(), // size in bytes 
				data.data(),				 // actual data
				GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	ShaderBuffer_POS(const std::vector<float> &v)
	{
		initResource();
		for (auto itm : v)
		{
			data.push_back(itm);
		}

		glGenBuffers(1, &bufferHandle);
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);
		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GLfloat)*data.size(), // size in bytes 
			data.data(),				 // actual data
			GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	
	~ShaderBuffer_POS()
	{
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	void deleteResource()
	{
		glDeleteBuffers(1, &bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};

class ShaderBuffer_POS_COL : public ShaderBuffer
{
	GLuint bufferHandle[2];
	GLuint vaoHandle;

	std::vector <GLfloat> position_data;
	std::vector <GLfloat> color_data;

public:
	ShaderBuffer_POS_COL()
	{
		initResource();
	}

	ShaderBuffer_POS_COL(const std::vector<float> &v1, const std::vector<float> &v2)
	{
		initResource();
		for (auto itm : v1)
		{
			position_data.push_back(itm);
		}

		for (auto itm : v2)
		{
			color_data.push_back(itm);
		}

		glGenVertexArrays(1, &vaoHandle);
		glGenBuffers(2, bufferHandle);
		glBindVertexArray(vaoHandle);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*position_data.size(), position_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*color_data.size(), color_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	~ShaderBuffer_POS_COL()
	{
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	void deleteResource()
	{
		glDeleteBuffers(1, bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};

class ShaderBuffer_POS_UV : public ShaderBuffer
{
	GLuint bufferHandle[2];
	GLuint vaoHandle;

	std::vector <GLfloat> position_data;
	std::vector <GLfloat> uv_data;

public:
	ShaderBuffer_POS_UV()
	{
		initResource();
	}

	ShaderBuffer_POS_UV(
		const std::vector<float> &v1,
		const std::vector<float> &v3
		)
	{
		initResource();
		for (auto itm : v1)
		{
			position_data.push_back(itm);
		}

		for (auto itm : v3)
		{
			uv_data.push_back(itm);
		}

		glGenVertexArrays(1, &vaoHandle);
		glGenBuffers(2, bufferHandle);
		glBindVertexArray(vaoHandle);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*position_data.size(), position_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*uv_data.size(), uv_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	~ShaderBuffer_POS_UV()
	{
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	void deleteResource()
	{
		glDeleteBuffers(3, bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};


class ShaderBuffer_POS_COL_UV : public ShaderBuffer
{
	GLuint bufferHandle[3];
	GLuint vaoHandle;

	std::vector <GLfloat> position_data;
	std::vector <GLfloat> color_data;
	std::vector <GLfloat> uv_data;

public:
	ShaderBuffer_POS_COL_UV()
	{
		initResource();
	}

	ShaderBuffer_POS_COL_UV(
							const std::vector<float> &v1, 
							const std::vector<float> &v2,
							const std::vector<float> &v3
							)
	{
		initResource();
		for (auto itm : v1)
		{
			position_data.push_back(itm);
		}

		for (auto itm : v2)
		{
			color_data.push_back(itm);
		}

		for (auto itm : v3)
		{
			uv_data.push_back(itm);
		}

		glGenVertexArrays(1, &vaoHandle);
		glGenBuffers(3, bufferHandle);
		glBindVertexArray(vaoHandle);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*position_data.size(), position_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*color_data.size(), color_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*uv_data.size(), uv_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	~ShaderBuffer_POS_COL_UV()
	{
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	void deleteResource()
	{
		glDeleteBuffers(3, bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};

class ShaderBuffer_POS_IND : public ShaderBuffer
{
	GLuint bufferHandle;
	GLuint vaoHandle;
	GLuint indexHandle;

	std::vector <GLfloat>	position_data;
	std::vector <GLushort>	index_data;
public:
	ShaderBuffer_POS_IND()
	{
		initResource();
	}

	ShaderBuffer_POS_IND
		(
		const std::vector<float>    &v1,
		const std::vector<GLushort> &v2
		)
		
	{
		initResource();

		for (auto itm : v1)
		{
			position_data.push_back(itm);
		}
		for (auto itm : v2)
		{
			index_data.push_back(itm);
		}

		glGenBuffers(1, &bufferHandle);
		glGenBuffers(1, &indexHandle);

		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);
			//position buffer
			glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*position_data.size(), position_data.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
			glEnableVertexAttribArray(0);
			
			//indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*index_data.size(), index_data.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	~ShaderBuffer_POS_IND()
	{
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	void deleteResource()
	{
		glDeleteBuffers(1, &bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};

class ShaderBuffer_POS_IND_COL : public ShaderBuffer
{
	GLuint bufferHandle[2]; // for position and color
	GLuint vaoHandle;
	GLuint indexHandle;

	std::vector <GLfloat>	position_data;
	std::vector <GLushort>	index_data;
	std::vector <GLfloat>	color_data;
public:
	ShaderBuffer_POS_IND_COL()
	{
		initResource();
	}

	ShaderBuffer_POS_IND_COL
		(
			const std::vector<GLfloat>		&v1,
			const std::vector<GLushort>		&v2,
			const std::vector<GLfloat>		&v3
		)
	{
		initResource();

		for (auto itm : v1)
		{
			position_data.push_back(itm);
		}
		for (auto itm : v2)
		{
			index_data.push_back(itm);
		}
		for (auto itm : v3)
		{
			color_data.push_back(itm);
		}

		glGenBuffers(2,  bufferHandle);
		glGenBuffers(1, &indexHandle);

		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);
		//position buffer
		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*position_data.size(), position_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(0);
		//color buffer
		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*color_data.size(), color_data.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(1);

		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat)*index_data.size(), index_data.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	~ShaderBuffer_POS_IND_COL()
	{
	}

	void initResource()
	{
		//printf("buffer initResource\n");
	}

	void deleteResource()
	{
		glDeleteBuffers(2, bufferHandle);
		glDeleteVertexArrays(1, &vaoHandle);
		//printf("buffer deleteResource\n");
	}

	GLuint getVAOHandle()
	{
		return vaoHandle;
	}
};

#endif