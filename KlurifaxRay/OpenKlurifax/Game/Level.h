#pragma once
#include "LevelSpline.h"
#include "../Base/ModelBank.h"
#include <vector>
#include <glm/glm.hpp>
#include "../Graphics/RenderQueue.h"
#include "../Graphics/Vertex.h"
using glm::vec3;
using glm::vec2;
using std::vector;

#define PLANE_WIDTH 40.0f
class Level
{
public:
	Level();
	~Level();
	void GenerateLevel();
	void Draw(RenderQueue* rq);

	void SetSpline(LevelSpline& spline);
private:
	LevelSpline m_Spline; //start with one spline
	ModelHandle m_Model;
};
