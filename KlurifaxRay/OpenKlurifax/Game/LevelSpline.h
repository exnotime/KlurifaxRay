#pragma once
#include "../Base/Spline.h"
#include "glm/glm.hpp"
using glm::vec3;
using std::vector;
class  LevelSpline
{
public:
	LevelSpline();
	~LevelSpline();
	void AddPoint(glm::vec3 pos, glm::vec3 normal);
	void Calc();

	vec3 GetNormal(float x);
	vec3 GetPos(float x);
private:
	Spline m_Spline;
	vector<vec3> m_Positions;
	vector<vec3> m_Normals;
	vector<float> m_Lengths;
};

