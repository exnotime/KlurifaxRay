#include "LevelSpline.h"

LevelSpline::LevelSpline(){

}

LevelSpline::~LevelSpline(){

}

void LevelSpline::AddPoint(vec3 pos, vec3 normal){
	m_Spline.AddPoint(pos);
	m_Positions.push_back(pos);
	m_Normals.push_back(glm::normalize(normal));
}

void LevelSpline::Calc(){
	m_Spline.CalcSpline();
	//Calc length
	int nrOfPoints = m_Spline.GetNrOfPoints();
	float max = 1.0f / nrOfPoints;
	float step = max / 100.0f;
	vec3 pos0, pos1;
	float d = 0.0f;
	float length = 0.0f;

	for (int i = 0; i < nrOfPoints - 1; ++i){
		for (; d < max * (i + 1); d += step){
			pos0 = m_Spline.GetPos(d);
			pos1 = m_Spline.GetPos(d + step);
			length += glm::length(pos1 - pos0);
		}
		m_Lengths.push_back(length);
		length = 0;
	}
}

vec3 LevelSpline::GetNormal(float x){
	if (x >= 1.0f){
		return GetNormal(0.9999999f);
	}
	else if (x < 0.0f){
		return GetNormal(0.0f);
	}
	vec3 nor0, nor1;
	float p = x * (m_Positions.size() - 1) ;
	int num = (int)p;
	float lerp = (p - num);

	nor0 = m_Normals[num];
	nor1 = m_Normals[num + 1];

	return (nor1 * lerp) + (nor0 * (1.0f - lerp));
}

vec3 LevelSpline::GetPos(float x){
	return m_Spline.GetPos(x);
}