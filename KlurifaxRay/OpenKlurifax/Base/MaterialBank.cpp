#include "MaterialBank.h"
#include <fstream>
#include <iostream>
#include "../Graphics/Model.h"
MaterialBank::MaterialBank(){
	m_MaterialBuffer = 0;
	glGenBuffers(1, &m_MaterialBuffer);
}

MaterialBank::~MaterialBank(){
	glDeleteBuffers(1, &m_MaterialBuffer);
}

MaterialBank& MaterialBank::GetInstance(){
	static MaterialBank m_Bank;
	return m_Bank;
}

TextureHandle MaterialBank::LoadTexture(const char* filename){
	TextureHandle id = ++m_Numerator;
	if (!m_Textures[id].GetLoaded()){
		if (!m_Textures[id].Init(filename, TEXTURE_2D)){
			printf("Error loading texture filename:%s\n", filename);
			return -1;
		}
	} else{
		return id;
	}
	return id;
}



TextureHandle MaterialBank::LoadCubeMap(const char* filename){
	TextureHandle id = ++m_Numerator;
	if (!m_Textures[id].GetLoaded()){
		if (!m_Textures[id].Init(filename, CUBE_TEXTURE)){
			printf("Error loading texture filename: %s\n", filename);
			return -1;
		}
	}
	else{
		return id;
	}

	return id;
}

TextureHandle MaterialBank::LoadRawTexture(int width, int height, int channels, const char* filename){
	TextureHandle id = ++m_Numerator;
	if (!m_Textures[id].GetLoaded()){
		std::streampos size;
		std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open()){
			size = file.tellg();
			char* data = new char[size];
			file.seekg(0, std::ios::beg);
			file.read(data, size);
			file.close();
			m_Textures[id].Init(data, 1, width, height, TEXTURE_2D);
			m_Textures[id].SetFilename(filename);
			return id;
		}
		else{
			printf("Could not find raw texture %s\n", filename);
			return -1;
		}
	}
	else{
		return id;
	}

}

Texture* MaterialBank::FetchTexture(int TextureHandle){
	return &m_Textures[TextureHandle];
}

Texture* MaterialBank::FetchTexture(const char* filename){
	for (map<TextureHandle, Texture>::iterator it = m_Textures.begin(); it != m_Textures.end();++it){
		if (std::strcmp(filename, it->second.GetFilename().c_str()) == 0){
			return &it->second;
		}
	}
	//texture does not excist in this bank yet
	//so load it, this behaviour is little volatile since it could have wanted to fetch a raw or cubemap
	//TODOHJ: Add to Logger
	return FetchTexture(LoadTexture(filename));
}

void MaterialBank::LoadMaterials(Model& model, string filename, const aiScene* scene){
	model.MaterialBasePointer = static_cast<int>(m_Materials.size());
	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
		Material modelMat;
		const aiMaterial* mat = scene->mMaterials[i];
		//get name
		aiString name;
		mat->Get(AI_MATKEY_NAME, name);
		modelMat.SetName(string(name.C_Str()) );
		//diffuse albedo
		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				string fullpath = GetDir(filename) + path.data;
				TextureHandle tex = LoadTexture(fullpath.c_str());
				modelMat.SetDiffuseTexture(tex);
			}
		}
		//normal map
		if (mat->GetTextureCount(aiTextureType_HEIGHT) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_HEIGHT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				string fullpath = GetDir(filename) + path.data;
				TextureHandle tex = LoadTexture(fullpath.c_str());
				modelMat.SetNormalTexture(tex);
			}
		}
		//displacement map
		//if this is used a tesselation shader should be used to create extra triangles.
		if (mat->GetTextureCount(aiTextureType_DISPLACEMENT) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_DISPLACEMENT, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				string fullpath = GetDir(filename) + path.data;
				TextureHandle tex = LoadTexture(fullpath.c_str());
				modelMat.SetDisplacmentTexture(tex);
			}
		}
		//Roughness map
		//if this is used multiply the resulting Specular power with the value from this to create a correct surface. 
		if (mat->GetTextureCount(aiTextureType_SHININESS) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				string fullpath = GetDir(filename) + path.data;
				TextureHandle tex = LoadTexture(fullpath.c_str());
				modelMat.SetGlossTexture(tex);
			}
		}
		//Lightmap for baked global illumination.
		if (mat->GetTextureCount(aiTextureType_LIGHTMAP) > 0){
			aiString path;
			if (mat->GetTexture(aiTextureType_LIGHTMAP, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				string fullpath = GetDir(filename) + path.data;
				TextureHandle tex = LoadTexture(fullpath.c_str());
				modelMat.SetLightTexture(tex);
			}
		}
		aiColor3D diff;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
		aiColor3D spec;
		mat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
		float shiny = 0.0f;
		mat->Get(AI_MATKEY_SHININESS, shiny);
		if (shiny == 0.0f) shiny = 1.0f;

		float d = (diff.r + diff.g + diff.b) * 0.333333f;
		float s = (spec.r + spec.g + spec.b) * 0.333333f;
		modelMat.SetDiffuseFactor(d);
		modelMat.SetSpecularFactor(s);
		
		modelMat.SetSpecularPower(shiny);
		modelMat.SetBaseColor(vec3(1,1,1));
		modelMat.SetFresnelFactor(0.01f);
		modelMat.SetGlossFactor(0.0f);
		modelMat.SetMetalicFactor(0.0f);
		modelMat.SetReflectiveColor(vec3(0));
		m_Materials.push_back(modelMat);
	}
}

string MaterialBank::GetDir(string& filename)
{
	bool found = false;
	for (int i = static_cast<int>(filename.size()); i > 0; i--)
	{
		if (filename.c_str()[i] == '/' || filename.c_str()[i] == '\\' )
			found = true;
		if (!found)
		{
			filename.erase(i);
		}

	}
	return filename;
}

void MaterialBank::BuildMaterialBuffer(){
	SurfaceMaterial* mats = new SurfaceMaterial[(int)m_Materials.size()];
	for (int i = 0; i < m_Materials.size(); ++i){
		mats[i] = m_Materials[i].GetSurfaceMaterial();
	}
	
	glBindBuffer(GL_UNIFORM_BUFFER, m_MaterialBuffer);
	glBufferData(GL_UNIFORM_BUFFER, (int)m_Materials.size() * sizeof(SurfaceMaterial), mats, GL_DYNAMIC_DRAW);
	delete[] mats;
}

GLuint MaterialBank::GetMaterialBuffer(){
	return m_MaterialBuffer;
}


Material& MaterialBank::GetMaterial(int matIndex){
	return m_Materials[matIndex];
}

void MaterialBank::UpdateMaterial(int matIndex, Material& mat){
	m_Materials[matIndex] = mat;
}

map<TextureHandle, Texture> MaterialBank::GetTextures(){
	return m_Textures;
}