#pragma once
#include <glm\glm.hpp>
using glm::vec3;
using glm::vec4;

enum LIGHT_TYPE
{
	POINTLIGHT,
	DIRLIGHT,
	SPOTLIGHT
};

struct PointLight
{
	vec3 Position;
	float Range;
	vec4 Diffuse;
	vec4 Specular;
	vec4 Ambient;
	vec3 Att;
	float padd;
};

struct DirLight
{
	vec4 Direction;
	vec4 Diffuse;
	vec4 Specular;
	vec4 Ambient;
};

struct SpotLight
{
	vec3 Direction;
	float Cutoff; //angle of the light, store as cosine
	vec4 Diffuse;
	vec4 Specular;
	vec4 Ambient;
	vec3 Att;
	float Range;
	vec3 Position;
	float SpotExp; // falloff from center of spot
};


