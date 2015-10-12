#include "Spline.h"
float GetValX(vec3 p){ return p.x; }
float GetValY(vec3 p){ return p.y; }
float GetValZ(vec3 p){ return p.z; }
Spline::Spline()
{
}

Spline::~Spline()
{
}

vector<Cubic> Spline::CalcNaturalCubic3D(vector<vec3> points, float(*GetVal)(vec3 p)){
	vector<Cubic> cubics;
	int num = points.size() - 1;

	float* gamma = new float[num + 1];
	float* delta = new float[num + 1];
	float* D = new float[num + 1];

	gamma[0] = 1.0f / 2.0f;
	for (int i = 1; i < num; ++i){
		gamma[i] = 1.0f / (4.0f - gamma[i - 1]);
	}
	gamma[num] = 1.0f / (2.0f - gamma[num - 1]);

	float p0 = GetVal(points[0]);
	float p1 = GetVal(points[1]);

	delta[0] = 3.0f * (p1 - p0) * gamma[0];
	for (int i = 1; i < num; ++i){
		float p0 = GetVal(points[i - 1]);
		float p1 = GetVal(points[i + 1]);
		delta[i] = (3.0f * (p1 - p0) - delta[i - 1]) * gamma[i];
	}
	p0 = GetVal(points[num - 1]);
	p1 = GetVal(points[num]);
	delta[num] = (3.0f * (p1 - p0) - delta[num - 1]) * gamma[num];

	D[num] = delta[num];
	for (int i = num - 1; i >= 0; --i){
		D[i] = delta[i] - gamma[i] * D[i + 1];
	}
	cubics.clear();
	for (int i = 0; i < num; ++i){
		p0 = GetVal(points[i]);
		p1 = GetVal(points[i + 1]);
		Cubic c = Cubic(p0, D[i],
			(3 * (p1 - p0) - 2 * D[i] - D[i + 1]),
			(2 * (p0 - p1) + D[i] + D[i + 1])
			);
		cubics.push_back(c);
	}
	delete[] gamma;
	delete[] delta;
	delete[] D;
	return cubics;
}

void Spline::AddPoint(vec3& point){
	m_Points.push_back(point);
}

void Spline::CalcSpline(){
	m_XCubic = CalcNaturalCubic3D(m_Points, &GetValX);
	m_YCubic = CalcNaturalCubic3D(m_Points, &GetValY);
	m_ZCubic = CalcNaturalCubic3D(m_Points, &GetValZ);
}

vec3 Spline::GetPos(float x){
	if (x >= 1.0f){
		return GetPos(0.9999999f);
	}
	float p = x * m_XCubic.size();
	int cubicNum = (int)p;
	float cubicPos = (p - cubicNum);
	vec3 pos;
	pos.x = m_XCubic[cubicNum].Eval(cubicPos);
	pos.y = m_YCubic[cubicNum].Eval(cubicPos);
	pos.z = m_ZCubic[cubicNum].Eval(cubicPos);
	return pos;
}

void Spline::Clear(){
	m_Points.clear();
}

int Spline::GetNrOfPoints(){
	return static_cast<int>(m_Points.size());
}
