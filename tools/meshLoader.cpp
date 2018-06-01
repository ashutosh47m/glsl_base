
#include "meshLoader.h"

using namespace mesher;

instancing *ml_instance;
meshLoader::meshLoader(const char * file, const char* textPath, bool tp, instancing *instance)
{
	if(tp) saveUV=true;
	else saveUV=false;

	if(instance != NULL)
	{
		/*
		if(instance->i_positions.size()>0 && instance->i_colors.size()>0)
		{
			instancingType = PC_I; // position, & color instancing
			// position & color instancingv 
		}
		else if(instance->i_positions.size()>0)
		{
			instancingType = P_I; // position instancing
		}
		else if(instance->i_colors.size()>0)
		{
			instancingType = C_I; // color instancing
		} */
		ml_instance=instance;
	}
	else
		ml_instance=NULL;

	texturePath = textPath;
    Assimp::Importer imp;
    const aiScene* scene = imp.ReadFile
    (
        file,
        aiProcess_GenSmoothNormals|
        aiProcess_Triangulate     |
        aiProcess_CalcTangentSpace|
        aiProcess_FlipUVs
    );

	if(scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        printf("MODELS wasn't loaded %s\n",file);
        return;
    }
    recProcess(scene->mRootNode, scene);
}

void meshLoader::recProcess(aiNode *node, const aiScene* scene)
{
    //processing
	numMeshes = node->mNumMeshes;
    for(unsigned int i=0; i<node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		numVertices = mesh->mNumVertices;
        processMesh(mesh, scene);
    }

    //recur logic
    for(unsigned int i=0; i< node->mNumChildren; i++)
        recProcess (node->mChildren[i], scene); 
 }


void meshLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector <vertexData> vdata;
    std::vector <GLuint> indices;
    std::vector <textureData> tdata;

    aiColor4D col;

	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
    aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col );


    glm::vec3 defaultColor(1.0f, 1.0f, 1.0f);

    //-----------------------VERTEXDATA
    for(unsigned int i=0; i<mesh->mNumVertices; i++) 
    {
        vertexData tmp;
        glm::vec3 tvec;

        //position
        tvec.x = mesh->mVertices[i].x;
        tvec.y = mesh->mVertices[i].y;
        tvec.z = mesh->mVertices[i].z;
        tmp.position = tvec;
		meshVertexPosition.push_back(tmp.position);

        //normal
        tvec.x = mesh->mNormals[i].x;
        tvec.y = mesh->mNormals[i].y;
        tvec.z = mesh->mNormals[i].z;
        tmp.normal = tvec;

        //tangents
        if(mesh->mTangents)
        {
            tvec.x = mesh->mTangents[i].x;
            tvec.y = mesh->mTangents[i].y;
            tvec.z = mesh->mTangents[i].z;
        }
        else
        {
            tvec.x = 0;
            tvec.y = 0;
            tvec.z = 0;
        }
        tmp.tangent= glm::vec3(tvec);

        //color
        if(mesh->mColors[0])
        {
            /*
                FYI, this is not a material color, 
                this is the color you will get when you will features of blender such as
                vertex painting.
            */
            tvec.x = mesh->mColors[0][i].r;
            tvec.y = mesh->mColors[0][i].g;
            tvec.z = mesh->mColors[0][i].b;
        }
        else
        {
            tvec = defaultColor;
        }
        tmp.color= tvec;
        
        //UV
        if(mesh->mTextureCoords[0]!=NULL)
        {
            tvec.x = mesh->mTextureCoords[0][i].x;
            tvec.y = 1-(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            tvec.x = 0.0f;
            tvec.y = 0.0f;
            tvec.z = 0.0f;
        }
        tmp.U= tvec.x;
        tmp.V= tvec.y;

        vdata.push_back(tmp);
    }

    //-----------------------INDICES
    for(unsigned int i=0; i< mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0; j< face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

	std::string tex;
    for(unsigned int i=0; i<mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
    {
        aiString str;
        mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
        
        textureData tmp;

		tex = texturePath;
		tex.append(str.C_Str());

		loadTexture(tex, tmp.id);
        tmp.type = 0;
        tdata.push_back(tmp);
    }

	for(unsigned int i=0; i<mat->GetTextureCount(aiTextureType_HEIGHT); i++)
    {
        aiString str;
        mat->GetTexture(aiTextureType_HEIGHT, i, &str);
        
        textureData tmp;

		tex = texturePath;
		tex.append(str.C_Str());

		loadTexture(tex, tmp.id);
        tmp.type = 0;
        tdata.push_back(tmp);
    }
//	if(ml_instance!=NULL)
   // meshes.push_back(new meshData(&vdata, &indices, &tdata, ml_instance) );
	//else
	if(saveUV)
	{
		for (std::vector<mesher::vertexData>::iterator it = vdata.begin() ; it != vdata.end(); ++it)		
		{
			glm::vec2 temp;
			temp.x = it->U;
			temp.y = it->V;
			terrain_UV.push_back(temp);
		}
	}

    meshes.push_back(new meshData(&vdata, &indices, &tdata));
} 

meshLoader::~meshLoader()
{
    for(unsigned int i=0; i<meshes.size(); i++)
        delete meshes[i];
}

void meshLoader::draw(ShaderProgram*& glProg, instancing *instance, GLuint instance_count)
{
	glUseProgram(glProg->getShaderProgramHandle());
	unsigned int i=0;

    for(; i<meshes.size(); i++)
        meshes[i]->draw(glProg, instance,instance_count);
}

//std::vector<mesh*> & getMeshes()
//{
//	return meshes;
//}


/*
    this is another varient of loadTexture()
    this method will take a file name as an argument, it will load the file and will return the texture id
*/

GLuint meshLoader::loadTexture(std::string file, GLuint& id)
{	
	float aniso=16.0f;
	/*
	tools::image_handler texture;
	texture.location=file;

	if(texture.location.substr(texture.location.length()-4, texture.location.length()).compare(".tga")==0)
	{
		glGenTextures(1, &id);
//		glActiveTexture(GL_TEXTURE0+texture.uniform_ID);
		glBindTexture(GL_TEXTURE_2D, id);

		texture.format = TGA;
		if(texture.LoadTGA((char*)texture.location.c_str())== false)
		{
			printf("texture not loaded\n");
			//MessageBox(NULL, L"Texture not loaded", NULL, NULL);
			//return false;	
		}	

		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

		glTexImage2D
		(
			GL_TEXTURE_2D, 
			0,
			texture.type, 
			texture.width, 
			texture.height, 
			0, 
			texture.type, 
			GL_UNSIGNED_BYTE, 
			texture.imageData
		);
	}
	else if(texture.location.substr(texture.location.length()-4, texture.location.length()).compare(".jpg")==0 
		|| texture.location.substr(texture.location.length()-4, texture.location.length()).compare(".jpeg")==0)
	{
		texture.format = JPG;
	    ILboolean success;
		ilInit();
		ilGenImages(1, &texture.jpg_id);
		ilBindImage(texture.jpg_id);
		std::wstring t(texture.location.begin(), texture.location.end());

		success = ilLoadImage(t.c_str());
		if (success)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); // Convert every colour component into
																// unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA 
			if (!success)
			{
				printf("image conversion failed\n");
				//MessageBox(NULL, L"image conversion failed", NULL, NULL);
			}
			glGenTextures(1, &id);
//			glActiveTexture(GL_TEXTURE0+texture.uniform_ID);
			glBindTexture(GL_TEXTURE_2D, id);
        
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
			printf("texture not loaded\n");
			//MessageBox(NULL, L"Texture not loaded", NULL, NULL);
			//return false;
		}
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// Linear Filtered
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   // Linear Filtered

	//prog.setUniform(texture.uniform_var.c_str(), texture.uniform_ID);	
	*/
	return id;
}


//generate VBO and other data in this constructor
meshData::meshData(std::vector <vertexData> *vd, std::vector <GLuint> *id, std::vector <textureData> *td)
{
	onceOnly=false;
	vdata	= *vd;
	indices = *id;
	if(td)
		tdata	= *td;

	GLuint offset = 0;

/*	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if(ml_instance!=NULL)
		glBufferData(GL_ARRAY_BUFFER, (vdata.size()*sizeof(vertexData)) + (ml_instance->i_positions.size()*sizeof(glm::vec4)) , NULL, GL_STATIC_DRAW);
	else
	glBufferData(GL_ARRAY_BUFFER, (vdata.size()*sizeof(vertexData)) , NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vdata.size()*sizeof(vertexData), &vdata[0]);
	offset += vdata.size()*sizeof(vertexData);

	if(ml_instance!=NULL)
	glBufferSubData(GL_ARRAY_BUFFER, 0, (ml_instance->i_positions.size()*sizeof(glm::vec4)), &ml_instance->i_positions[0].x);

*/

	glGenVertexArrays( 1, &meshVAOHandle );
	glBindVertexArray(meshVAOHandle );

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vdata.size()*sizeof(vertexData)) , NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vdata.size()*sizeof(vertexData), &vdata[0]);

	if(ml_instance!=NULL)
	{	
		glGenBuffers(1, &iVBO);
		glBindBuffer(GL_ARRAY_BUFFER, iVBO);
		glBufferData(GL_ARRAY_BUFFER, (ml_instance->i_positions.size()*sizeof(glm::vec4)) , &ml_instance->i_positions[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(5);
 		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribDivisor(5, 1);
	}

	glGenBuffers(1, &INDBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INDBO);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER, 
		indices.size()*sizeof(GLuint), 
		&indices[0], GL_STATIC_DRAW
	);

	int vertex = 0;//glGetAttribLocation(glProg.getHandle(), "in_model_v3Position");
	int normal = 1;// glGetAttribLocation(glProg.getHandle(), "in_model_v3Normal");
	int tangent = 2;// glGetAttribLocation(glProg.getHandle(), "in_model_v3Tangent");
	int color = 3;// glGetAttribLocation(glProg.getHandle(), "in_model_v3Color");
	int UV = 4;// glGetAttribLocation(glProg.getHandle(), "in_model_v2UV");
	int i_positions = 5;//glGetAttribLocation(glProg.getHandle(), "in_model_v4instancedPositions");      //location 8


	glEnableVertexAttribArray(vertex);
	glEnableVertexAttribArray(normal);
	glEnableVertexAttribArray(tangent);
	glEnableVertexAttribArray(color);
	glEnableVertexAttribArray(UV);
	//now bind the textures we have stored in tdata vector..
	//the tdata was loaded in the constructor of this class..

	//_____________________UPLOAD rest of data TO GPU__________________

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INDBO);

//	if(onceOnly==false)

	glVertexAttribPointer( vertex, 3,	GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);
	glVertexAttribPointer( normal,3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)((sizeof(glm::vec3))) );
	glVertexAttribPointer( tangent, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)((2*sizeof(glm::vec3))) );
	glVertexAttribPointer( color,3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)((3*sizeof(glm::vec3))) );
	glVertexAttribPointer( UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)((4*sizeof(glm::vec3))));
	
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

meshData::~meshData()
{
	glDeleteBuffers(1, &VBO); 
	glDeleteBuffers(1, &INDBO);

	for (unsigned int i = 0; i<tdata.size(); i++)
	{
		glDeleteTextures(1, &tdata[i].id);
	}
}

void meshData::draw(ShaderProgram*& glProg, instancing *instance, GLuint instance_count)
{
	glBindVertexArray(meshVAOHandle );
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INDBO);

	//_____________________UPLOAD TEXTURES TO GPU__________________
	std::string temp = "texture";

	GLuint i;
	for(i=0; i<tdata.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, tdata[i].id);

		std::string s = temp+(char)(i+'0');
		const char* t = s.c_str();
		glProg->setUniform(t, i);
	}
	if(instance!=NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, iVBO);
		//glBufferData(GL_ARRAY_BUFFER, (ml_instance->i_positions.size()*sizeof(glm::vec4)) , &ml_instance->i_positions[0], GL_STATIC_DRAW);
//  		glPointSize(4);
 		glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0, instance_count);
//		glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, instance_count);
//		glDrawElementsInstanced(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0, instance_count);
	}
	else if(instance==NULL)
	{
   	 	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
//  	glPointSize(4);
// 		glLineWidth(1.4f);
//		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
// 		glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	}

	//---------------------------------------------------------------------
	/* at this point our fng is complete.. and we can unbind and delete all the things we used. */
		// the things in " " calls should be defined in your vertex buffer.

	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}