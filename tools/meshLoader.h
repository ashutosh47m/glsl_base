#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <gl/glew.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "gfx_tools.h"

#include "constants.h"

#include <assimp/Importer.hpp> //c++ importer
#include <assimp/scene.h>          
#include <assimp/postprocess.h>    



//its okay to use "..." below, as we have all the files in the 'glsl_tools' direcotry
//#include "meshData.h"
namespace mesher
{
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
		std::vector <textureData> tdata;
		std::vector <GLuint>      indices; 
		
		GLuint VBO; 
		GLuint iVBO; 
		GLuint INDBO;

	  public:
		  bool onceOnly;

		  meshData (shaderprog &glProg,  std::vector <vertexData>* vd,  std::vector <GLuint> *id,  std::vector <textureData> *td = NULL);

		~meshData();
		void draw(shaderprog &glProg, tools::instancing *instance=NULL, GLuint instance_count=0);
	}; 

	class meshLoader
	{
	public:
		shaderprog glslProg; 
		std::vector <glm::vec2> terrain_UV; 
		glm::vec3 position;
		GLuint instance_count;
		int instancingType;
		std::string texturePath;
		int numVertices;
		int numMeshes;
		bool saveUV;
		meshLoader(const char *, const char *, shaderprog &glProg, bool tp=false, tools::instancing *instance = NULL);
		~meshLoader();
		std::vector <glm::vec3> meshVertexPosition;
		std::vector<meshData*> meshes; 
		void recProcess  (aiNode* ,const  aiScene* );
		void processMesh (aiMesh* ,const  aiScene* );
		void draw(shaderprog  &glProg,tools::instancing *instance = NULL, GLuint instance_count=0);
		GLuint loadTexture(std::string file, GLuint&);
	};

}
#endif