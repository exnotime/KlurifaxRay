#include "Sprite.h"

Sprite::Sprite(){

}

Sprite::Sprite(Texture* tex){
	Init(tex);
}

Sprite::Sprite(const char* filename){
	Init(filename);
}

Sprite::~Sprite(){
}

void Sprite::Init(const char* filename){
	m_Texture.Init(filename, TEXTURE_2D);
}

void Sprite::Init(Texture* tex){
	m_Texture = *tex;
}

void Sprite::Move(vec2 amount){
	m_Position += amount;
}

void Sprite::SetPos(vec2 pos){
	m_Position = pos;
}

void Sprite::Rotate(vec2 origin, float angle){
	m_Rotation.x = origin.x;
	m_Rotation.y = origin.y;
	m_Rotation.z = angle;
}

vec2 Sprite::GetPos(){
	return m_Position;
}

vec3 Sprite::GetRotation(){
	return m_Rotation;
}

Texture* Sprite::GetTexture(){
	return &m_Texture;
}