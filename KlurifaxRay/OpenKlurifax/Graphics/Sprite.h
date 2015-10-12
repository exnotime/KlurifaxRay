#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
using glm::vec2;
using glm::vec3;
class Sprite{
public:
	Sprite();
	Sprite(Texture* tex);
	Sprite(const char* filename);
	~Sprite();
	void Init(const char* filename);
	void Init(Texture* tex);
	void Move(vec2 amount);
	void SetPos(vec2 pos);
	void Rotate(vec2 origin, float angle); // TODO: Implement
	vec2 GetPos();
	vec3 GetRotation();
	Texture* GetTexture();
	
private:
	vec2 m_Position;
	Texture m_Texture;
	vec3 m_Rotation; //store as xy = point to ratate around and z as angle computation will be done in the shader
	vec2 m_Scale; //TODO: Implment
};