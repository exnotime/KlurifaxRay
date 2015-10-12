#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>

#define MaxSpheres 1000
#define MaxTriangles 1000
#define BufferSize MaxSpheres * sizeof(Sphere) + MaxTriangles * sizeof(Triangle)
#define TrianglesBufferPos MaxSpheres * sizeof(Sphere)

using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::vector;

struct Sphere
{
	vec3 c;
	float r;
	vec4 Color;
};

struct Triangle{
	vec4 p0;
	vec4 p1;
	vec4 p2;
	vec4 Color;
};

struct AABB{
	vec3 Bounds[2];
	vec4 Color;
};

class PrimitivesBuffer{
public:
	PrimitivesBuffer();
	~PrimitivesBuffer();

	void Init();
	void Build();
	void Bind(GLuint program, GLuint blockBinding);
	GLuint GetBuffer();

	void AddSphere(Sphere s);
	void AddTriangle(Triangle t);
	int nSpheres();
	int nTriangles();

	void GenerateAABB(vec3 minBounds, vec3 maxBounds);
	
private:
	GLuint m_Buffer;
	vector<Sphere> m_Spheres;
	vector<Triangle> m_Triangles;
};
