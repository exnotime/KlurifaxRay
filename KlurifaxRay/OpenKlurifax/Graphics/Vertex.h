#pragma once
#include <glm\glm.hpp>
using namespace glm;
/*-----------------------------------------
/This document provides a wide array of vertex types
/If a new vertex type is added it must be added to the VertexBuffer class 
------------------------------------------*/
//model with a single texture and lighting effects
struct VertexPosNormalTex
{
	VertexPosNormalTex(){

	}
	VertexPosNormalTex(vec3 p, vec3 n, vec2 uv){
		Position = p;
		Normal = n;
		TexCoord = uv;
	}
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
};
struct VertexPosNormalTexTangent
{
	VertexPosNormalTexTangent(){

	}
	VertexPosNormalTexTangent(vec3 p, vec3 n, vec2 uv,vec3 t){
		Position = p;
		Normal = n;
		TexCoord = uv;
		Tangent = t;
	}
	vec3 Position;
	vec3 Normal;
	vec2 TexCoord;
	vec3 Tangent;
};
//may be used for single color model or geometry shader based particle system
struct VertexPos
{
	vec3 Position;
};
//may be used for screenaligned quad and geometry without advanced lighting model
struct VertexPosTex
{
	vec3 Position;
	vec2 TexCoord;
};
//used for single colored models with lighting
struct VertexPosNormal
{
	vec3 Position;
	vec3 Normal;
};