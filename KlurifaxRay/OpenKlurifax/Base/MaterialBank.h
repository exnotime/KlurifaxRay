#pragma once
#include <map>
#include <string>
#include <assimp/scene.h>
#include "../Graphics/Texture.h"
#include "../Graphics/Model.h"
#include "../Graphics/Material.h"
using std::map;
using std::string;
#define g_MaterialBank MaterialBank::GetInstance()
#define MaxMaterials 100
typedef int TextureHandle;
class MaterialBank
{
public:
	~MaterialBank();
	static MaterialBank& GetInstance();
	TextureHandle LoadTexture(const char* filename);
	TextureHandle LoadCubeMap(const char* filename);
	TextureHandle LoadRawTexture(int width, int height, int channels, const char* filename);
	Texture* FetchTexture(int TextureHandle);
	Texture* FetchTexture(const char* filename);
	void LoadMaterials(Model& model, string filename, const aiScene* scene);
	void BuildMaterialBuffer();
	GLuint GetMaterialBuffer();
	Material& GetMaterial(int matIndex);
	void UpdateMaterial(int matIndex, Material& mat);
	map<TextureHandle, Texture> GetTextures();
private:
	MaterialBank();
	int m_Numerator = 0;
	map<TextureHandle, Texture> m_Textures;
	GLuint m_MaterialBuffer;
	vector<Material> m_Materials;
	string GetDir(string& filename);
};