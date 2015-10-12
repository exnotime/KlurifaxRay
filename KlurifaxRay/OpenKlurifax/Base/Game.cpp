#include "Game.h"
#include "MaterialBank.h"
#include "ModelBank.h"
Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(LightEngine* le){
	input = Input::GetInstance();
	input->Initialize();
	
	//plane = g_ModelBank.LoadModel("Content/plane.obj"); //Tricount 2
	//sword = g_ModelBank.LoadModel("Content/kokiri sword/Kokiri Sword.obj"); //Tricount 44 
	//mamechi = g_ModelBank.LoadModel("Content/mamechi.obj"); //Tricount 662x
	//miku = g_ModelBank.LoadModel("Content/miku/miku.obj"); //Tricount 2865
	//Lucina = g_ModelBank.LoadModel("Content/LucinaResource/Lucina_Posed.obj"); //Tricount 17537x
	scene = g_ModelBank.LoadModel("Content/RayScene/Scene.obj"); //TriCount 4597 //VertexCount 2579
	//world = mat4x4(1.0f);
	//world = glm::translate(0.0f, 0.0f, 0.0f) * glm::scale(vec3(10));

	p = le->CreateDynamicPointLight();
	p->Ambient = vec4(0.1f);
	p->Diffuse = vec4(1.0f);
	p->Specular = vec4(1.0f);
	p->Position = vec3(0, 0, 0);
	p->Range = 500.0f;
	p->Att = vec3(0.7f, 0.0001f, 0.0f);
	
	/*
	PointLight pl;
	pl.Range = 50.0f;
	pl.Att = vec3(1.0f, 0.1f, 0.01f);
	pl.Diffuse = vec4(0.6f);
	pl.Specular = vec4(0.6f);
	pl.Ambient = vec4(0.01f);
	
	int LightCount = 14;
	for (int i = 0; i < LightCount; ++i){
		float a = (i / (float)LightCount) * 3.14159265359f * 2.0f;
		pl.Position = vec3(cos(a) + sin(a), abs(cos(a)) ,-sin(a) + cos(a)) * 20.0f;
		le->CreateStaticPointLight(pl);
	}
	*/

	dl = g_LightEngine.CreateDynamicDirLight();
	dl->Ambient = vec4(0.1f);
	dl->Diffuse = vec4(0.2f);
	dl->Specular = vec4(0.2f);
	dl->Direction = vec4(0.7f,1.0f,0.3f,0.0f);

	m_Cam.SetLens(0.5f, 300.0f, 16.0f / 12.0f, 75.0f / (16.0f / 12.0f));
	m_Cam.SetPosition(vec3(13.14f,-11.04f,-3.23f));
	m_Cam.LookAt(vec3(0, -5, 0));
	m_Cam.BuildViewMatrix();

	p->Position = m_Cam.GetPos() + vec3(2,2,2);
}

bool Game::Update(float deltaTime)
{
	float movespeed = 7.0f;
	input->Update();
	//first person cam
	if (input->KeyDown(SDL_SCANCODE_ESCAPE)){
		return true;
	}
	if (input->KeyDown(SDL_SCANCODE_W)){
		m_Cam.MoveRelative(0, 0, -movespeed * deltaTime);
	}
	if (input->KeyDown(SDL_SCANCODE_S)){
		m_Cam.MoveRelative(0, 0, movespeed * deltaTime);
	}
	if (input->KeyDown(SDL_SCANCODE_A)){
		m_Cam.MoveRelative(-movespeed * deltaTime, 0, 0);
	}
	if (input->KeyDown(SDL_SCANCODE_D)){
		m_Cam.MoveRelative(movespeed * deltaTime, 0, 0);
	}
	//rotation
	m_Cam.RotateY(input->MouseDeltaX() * deltaTime);
	m_Cam.Pitch(-input->MouseDeltaY()  * deltaTime);

	if (input->KeyDown(SDL_SCANCODE_SPACE)){
		printf("camera pos = %f %f %f\n", m_Cam.GetPos().x, m_Cam.GetPos().y, m_Cam.GetPos().z);
		p->Position = m_Cam.GetPos();
	}
	dl->Direction = (glm::rotate(0.5f, 0.0f, 1.0f, 0.0f) * dl->Direction);

	m_Cam.BuildViewMatrix();
	if (input->KeyDown(SDL_SCANCODE_ESCAPE)){
		return true;
	}
	return false;
}

void Game::Draw(RenderQueue* rq){
	rq->Begin(&m_Cam);
	rq->End();
}