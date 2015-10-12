#pragma once
#include <glm/glm.hpp>
#include "../Base/ModelBank.h"
#include "Texture.h"
#include "Model.h"
using std::vector;
using glm::mat4x4;

enum class RENDER_TYPE{
	MODEL,
	MODEL_TRANS,
	SPRITE
};

class RenderObject
{
public:
	RenderObject(){};
	virtual ~RenderObject(){};
	const RENDER_TYPE GetType() const { return m_Type; };
	void SetType(RENDER_TYPE type){ m_Type = type; };
protected:
	RENDER_TYPE m_Type;
};

class ModelObject : public RenderObject{
public:
	mat4x4 World;
	ModelHandle Model;
};