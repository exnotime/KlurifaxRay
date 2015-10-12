#include "Level.h"
#include "../Base/ModelBank.h"
#include "../Base/MaterialBank.h"
Level::Level(){

}

Level::~Level(){

}

void Level::GenerateLevel(){

	vector<VertexPosNormalTexTangent> Vertices;
	vector<unsigned int> Indices;
	//Generate first plane
	float step = 1.0f / 100.0f; // 100 planes
	vec3 p, p0, p1;
	vec3 n;
	vec3 tangent;
	vec3 pn;
	int numVertices = 0;
	float x;
	//first 2 points
	x = 0.0f;
	p = m_Spline.GetPos(x);
	n = m_Spline.GetNormal(x);
	pn = glm::normalize(m_Spline.GetPos(x + step) - p);
	tangent = glm::normalize(glm::cross(n, pn));
	p0 = p - (tangent * PLANE_WIDTH * 0.5f);
	p1 = p + (tangent * PLANE_WIDTH * 0.5f);

	Vertices.push_back(VertexPosNormalTexTangent(p0, n, vec2(0, 0),tangent));
	Vertices.push_back(VertexPosNormalTexTangent(p1, n, vec2(1, 0), tangent));
	numVertices += 2;

	for (int i = 1; i < 1000; i++){
		x = i / 1000.0f;
		p = m_Spline.GetPos(x);
		n = m_Spline.GetNormal(x);
		pn = glm::normalize(m_Spline.GetPos(x + step) - p);
		tangent = glm::normalize(glm::cross(n, pn));
		vec3 vertexNormal = glm::normalize(glm::cross(pn, tangent));
		p0 = p - (tangent * PLANE_WIDTH * 0.5f);
		p1 = p + (tangent * PLANE_WIDTH * 0.5f);
		Vertices.push_back(VertexPosNormalTexTangent(p0, vertexNormal, vec2(0, x), tangent));
		Vertices.push_back(VertexPosNormalTexTangent(p1, vertexNormal, vec2(1.0f, x), tangent));

		Indices.push_back(numVertices - 2);
		Indices.push_back(numVertices);
		Indices.push_back(numVertices - 1);

		Indices.push_back(numVertices);
		Indices.push_back(numVertices + 1);
		Indices.push_back(numVertices - 1);
		//double sided
		Indices.push_back(numVertices - 1);
		Indices.push_back(numVertices);
		Indices.push_back(numVertices - 2);

		Indices.push_back(numVertices - 1);
		Indices.push_back(numVertices + 1);
		Indices.push_back(numVertices);	

		numVertices += 2;
	}
	//generate model and materials
	Model model;
	m_Model = g_ModelBank.AddModel(Vertices, Indices, model);
	model.Name = "Level";
	model.MaterialBasePointer = 0;
	model.Meshes[0].Material = 0;
	g_ModelBank.UpdateModel(m_Model, model);
}

void Level::Draw(RenderQueue* rq){
	rq->Enqueue(glm::translate(1.0f, 1.0f, 1.0f), m_Model);
}

void Level::SetSpline(LevelSpline& spline){
	m_Spline = spline;
}