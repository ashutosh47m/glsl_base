#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <gl/glew.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <assimp/Importer.hpp> //c++ importer
#include <assimp/scene.h>          
#include <assimp/postprocess.h>    
#include "../resources/shader_program.h"
#include "IL/il.h"

//its okay to use "..." below, as we have all the files in the 'glsl_tools' direcotry
//#include "meshData.h"

namespace mesher
{
	struct instancing
	{
		std::vector <glm::vec4> i_positions;
		std::vector <glm::vec4> i_scale;
		GLuint i_count;
	};

	struct vertexData
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 color;
		float U,V;
	};

	
	struct textureData 
	{
		GLuint id;
		GLuint type;
	};
	

	class meshData
	{
		GLuint meshVAOHandle;
		std::vector <vertexData>  vdata; 
		std::vector <textureData>	tdata;
		std::vector <GLuint>      indices; 
		
		GLuint VBO; 
		GLuint iVBO; 
		GLuint INDBO;

	  public:
		  bool onceOnly;

		  meshData (std::vector <vertexData>* vd,  std::vector <GLuint> *id,  std::vector <textureData> *td = NULL);

		~meshData();
		void draw(ShaderProgram*& glProg, bool, instancing *instance=NULL, GLuint instance_count=0);
	}; 

	class meshLoader
	{
	public:
		std::vector <glm::vec2> terrain_UV; 
		glm::vec3 position;
		GLuint instance_count;
		int instancingType;
		std::string texturePath;
		int numVertices;
		int numMeshes;
		bool saveUV;
		meshLoader(const char *, const char *, bool tp=false, instancing *instance = NULL);
		~meshLoader();
		std::vector <glm::vec3> meshVertexPosition;
		std::vector<meshData*> meshes; 
		void recProcess  (aiNode* ,const  aiScene* );
		void processMesh (aiMesh* ,const  aiScene* );
		void draw(ShaderProgram*& glProg, bool, instancing *instance = NULL, GLuint instance_count=0);
		GLuint loadTexture(std::string file, GLuint&);
	};

	class meshImage
	{
		const int TgaTypeHeaderLength = 12;
		const unsigned char UncompressedTgaHeader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
		const unsigned char CompressedTgaHeader[12] = { 0,0,10,0,0,0,0,0,0,0,0,0 };

		enum TgaType
		{
			TgaUncompressed,
			TgaCompressed,
			TgaUnknown
		};

	public:
		GLubyte *imageData;                         // Image Data (Up To 32 Bits)
		GLuint  bpp;                                // Image Color Depth In Bits Per Pixel.
		GLuint  width;                              // Image Width
		GLuint  height;                             // Image Height
		GLuint  ui_texID;                              // Texture ID Used To Select A Textur+e
		GLuint  type;
		GLuint  imageSize;										// Used To Store The Image Size When Setting Aside Ram
		TgaType compression;
		GLuint  bytesPerPixel;
		std::string	uniform_var;
		GLuint uniform_ID;
		GLuint jpg_id;
		std::string location;
		GLuint format;
		//GLfloat heightMap[MAP_X][MAP_Z];
		bool LoadTGA(char *);             // Loads A TGA File Into Memory
		bool loadFromData(GLubyte *);
		bool LoadUncompressed(GLubyte *src, unsigned int size, unsigned int width, unsigned int height, unsigned int bpp);
		bool LoadCompressed(GLubyte *src, unsigned int width, unsigned int height, unsigned int bpp);
		void deleteImage();
		bool loadJPG(const char *);

		const int TGA_ID = 42314;
		const int JPG_ID = 12314;

		meshImage();
		~meshImage();
	};


	/*
	class sound
	{
	public:
		FMOD::System *system;
		FMOD_RESULT result;
		unsigned int version;
		int numDrivers;
		FMOD_SPEAKERMODE speakerMode;
		FMOD_CAPS caps;
		char name[256];
		sound::sound(); //init FMOD
		void FMODErrorCheck(FMOD_RESULT result);
		~sound();
	};
	*/
}
#endif