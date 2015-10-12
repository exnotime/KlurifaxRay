#include "RenderQueue.h"


RenderQueue::RenderQueue()
{
	m_CurrentScene = nullptr;
}

RenderQueue::~RenderQueue()
{
	if (m_CurrentScene) delete m_CurrentScene;
}

void RenderQueue::Begin(Camera* camera){
	if (m_CurrentScene) delete m_CurrentScene;
	m_CurrentScene = new RenderScene();
	if (m_CurrentScene->isActive){
		m_CurrentScene->Objects.clear();
	}
	m_CurrentScene->Camera = camera;
	m_CurrentScene->isActive = true;
}

void RenderQueue::End(){
	m_CurrentScene->isActive = false;
	std::sort(m_CurrentScene->Objects.begin(), m_CurrentScene->Objects.end(), &RenderQueue::CompareRenderObjects);
	m_Scenes.push_back(*m_CurrentScene);
}

void RenderQueue::Enqueue(const mat4x4 &world, ModelHandle& model){
	ModelObject* obj = new ModelObject();
	obj->Model = model;
	obj->World = world;
	obj->SetType( RENDER_TYPE::MODEL);
	m_CurrentScene->Objects.push_back(obj);
	m_CurrentScene->Programs.push_back(nullptr); //default program
}

void RenderQueue::Enqueue(Sprite& sprite){

}

void RenderQueue::Enqueue(RenderObject* object, IRenderProgram* program){
	m_CurrentScene->Objects.push_back(object);
	m_CurrentScene->Programs.push_back(program);
}

void RenderQueue::Clear()
{
	for (auto it : m_Scenes){
		for (auto objIt : it.Objects){
			if(objIt) delete objIt;
		}
		it.Objects.clear();
	}
	m_Scenes.clear();
}

vector<RenderScene>* RenderQueue::GetScenes(){
	return &m_Scenes;
}

bool RenderQueue::CompareRenderObjects(const RenderObject* lhs,const RenderObject* rhs){
	return lhs->GetType() < rhs->GetType();
}