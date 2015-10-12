#pragma once
#include <map>
#include <string>
#include "../Graphics/Texture.h"
using std::map;
using std::string;
typedef int TextureHandle;
/*
MaterialBank Loads and stores all textures.
Textures are fetched using an integer aquired from a hash function of a string(filename)
*/
class MaterialBank
{
public:
	MaterialBank();
	~MaterialBank();
	static MaterialBank* GetInstance();
	TextureHandle LoadTexture(const char* filename);
	TextureHandle LoadCubeMap(const char* filename);
	TextureHandle LoadRawTexture(int width, int height, int channels, const char* filename);
	Texture* FetchTexture(int TextureHandle);
	Texture* FetchTexture(const char* filename);
	void LoadMaterials(Model& model, string& filename, const aiScene* scene);

private:
	int m_Numerator = 0;
	map<TextureHandle, Texture> m_Textures;
	GLuint m_MaterialBuffer;
	vector<ModelMaterial> m_Materials;
};