#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Texture.h"
using glm::vec4;
using glm::vec3;
using glm::vec2;
using std::string;
//This struct is the one being packaged into a buffer
/*struct SurfaceMaterial{
	//General Color of the object
	//will be blended by the diffuseTexture
	vec3 BaseColor;
	//diffuse intensity
	float Diffuse;
	//specular intensity
	float Specular;
	//not all materials need a reflection texture
	//this color should be the aproximation of the general color in the area
	//Will be overwritten by the ReflectionTexture
	vec3 ReflectedColor;
	//affects the specular 
	float Fresnel;
	//A general smoothness of the object.
	//Will be overwritten by the GlossTexture
	float Gloss;
	//not implemented yet
	float Metalic;
	//size of the specular highlight
	float SpecularPower;
};
*/
struct SurfaceMaterial{
	vec4 BaseColorDiffuse;
	vec4 ReflectedColorSpecular;
	vec4 FresnelGlossMetalPower;
};

class Material
{
public:
	Material(){ };
	~Material(){ };
	//Getters
	string& GetName(){ return m_Name; }

	TextureHandle& GetDiffuseTexture()				{ return m_DiffuseTexture; }
	TextureHandle& GetNormalTexture()				{ return m_NormalTexture; }
	TextureHandle& GetDisplacmentTexture()			{ return m_DisplacementTexture; }
	TextureHandle& GetReflectionTexture()			{ return m_ReflectionTexture; }
	TextureHandle& GetGlossTexture()				{ return m_GlossTexture; }
	TextureHandle& GetLightTexture()				{ return m_LightTexture; }

	vec3 GetBaseColor()							{ return vec3(m_Material.BaseColorDiffuse.x, m_Material.BaseColorDiffuse.y, m_Material.BaseColorDiffuse.z); }
	vec3 GetReflectiveColor()						{ return vec3(m_Material.ReflectedColorSpecular.x, m_Material.ReflectedColorSpecular.y, m_Material.ReflectedColorSpecular.z);; }
	float& GetDiffuseFactor()						{ return m_Material.BaseColorDiffuse.w; }
	float& GetSpecularFactor()						{ return m_Material.ReflectedColorSpecular.w; }
	float& GetFresnelFactor()						{ return m_Material.FresnelGlossMetalPower.x; }
	float& GetGlossFactor()							{ return m_Material.FresnelGlossMetalPower.y; }
	float& GetMetalicFactor()						{ return m_Material.FresnelGlossMetalPower.z; }
	float& GetSpecularPower()						{ return m_Material.FresnelGlossMetalPower.w; }

	SurfaceMaterial& GetSurfaceMaterial()			{ return m_Material; }
	//Setters
	void SetName(string name) { m_Name = name; }
	void SetDiffuseTexture(TextureHandle& tex)		{ m_DiffuseTexture = tex; }
	void SetNormalTexture(TextureHandle& tex)		{ m_NormalTexture = tex; }
	void SetDisplacmentTexture(TextureHandle& tex)	{ m_DisplacementTexture = tex; }
	void SetReflectionTexture(TextureHandle& tex)	{ m_ReflectionTexture = tex; }
	void SetGlossTexture(TextureHandle& tex)		{ m_GlossTexture = tex; }
	void SetLightTexture(TextureHandle& tex)		{ m_LightTexture = tex; }

	void SetBaseColor(vec3 color)					{ m_Material.BaseColorDiffuse.x = color.x; m_Material.BaseColorDiffuse.y = color.y; m_Material.BaseColorDiffuse.z = color.z; }
	void SetReflectiveColor(vec3 color)				{ m_Material.ReflectedColorSpecular.x = color.x; m_Material.ReflectedColorSpecular.y = color.y; m_Material.ReflectedColorSpecular.z = color.z; }
	void SetDiffuseFactor(float d)					{ m_Material.BaseColorDiffuse.w = d; }
	void SetSpecularFactor(float s)					{ m_Material.ReflectedColorSpecular.w = s; }
	void SetFresnelFactor(float f)					{ m_Material.FresnelGlossMetalPower.x = f; }
	void SetGlossFactor(float g)					{ m_Material.FresnelGlossMetalPower.y = g; }
	void SetMetalicFactor(float m)					{ m_Material.FresnelGlossMetalPower.z = m; }
	void SetSpecularPower(float sp)					{ m_Material.FresnelGlossMetalPower.w = sp; }

private:
	SurfaceMaterial m_Material;
	TextureHandle m_DiffuseTexture = -1;
	TextureHandle m_NormalTexture = -1;
	TextureHandle m_DisplacementTexture = -1;
	TextureHandle m_ReflectionTexture = -1; //note: CubeMap
	TextureHandle m_GlossTexture = -1;
	TextureHandle m_LightTexture = -1;
	string m_Name;
};
