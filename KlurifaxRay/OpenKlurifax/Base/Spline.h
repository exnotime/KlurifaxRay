#pragma once
#include "glm/glm.hpp"
#include <vector>
using std::vector;
using glm::vec2;
using glm::vec3;

struct Cubic{
	float a, b, c, d;
	Cubic();
	Cubic(float a, float b, float c, float d){
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

	float Eval(float u){
		return (((d*u) + c) * u + b) * u + a;
	}
};


class Spline
{
public:
	Spline();
	~Spline();
	void CalcSpline();
	void AddPoint(vec3& point);
	vec3 GetPos(float x);
	void Clear();
	int GetNrOfPoints();
private:
	vector<Cubic> CalcNaturalCubic3D(vector<vec3> points, float(*GetVal)(vec3 point));

	vector<vec3> m_Points;
	vector<Cubic> m_XCubic, m_YCubic, m_ZCubic;

};

