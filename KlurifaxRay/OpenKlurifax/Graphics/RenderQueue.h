#pragma once
#include <vector>
#include <algorithm>
#include "RenderObjects.h"
#include "IRenderProgram.h"
#include "Camera.h"
#include "Lights.h"
#include "Sprite.h"
#include "../Base/ModelBank.h"
#define DATA_SIZE 1024 * 1024 * 8
struct RenderScene
{
	Camera* Camera;
	vector<RenderObject*> Objects;
	vector<IRenderProgram*> Programs;
	bool isActive = false;
};

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	void Begin(Camera* camera /*, viewport*/);
	void End();
	void Enqueue(const mat4x4& world, ModelHandle& model);
	void Enqueue(Sprite& sprite);
	void Enqueue(RenderObject* object, IRenderProgram* program);

	void Clear();
	vector<RenderScene>* GetScenes();
private:
	static bool CompareRenderObjects(const RenderObject *lhs,const RenderObject *rhs);
	RenderScene* m_CurrentScene;
	vector<RenderScene> m_Scenes;
};

