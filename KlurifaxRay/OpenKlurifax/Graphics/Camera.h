#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform2.hpp>
#include <glm\gtx\quaternion.hpp>
using namespace glm;
class Camera
{
private:
	mat4 m_View;
	mat4 m_Projection;
	vec3 m_Position;
	vec3 m_Right;
	vec3 m_Look;
	vec3 m_Up;
	float m_Fov;
public:
	Camera();
	~Camera();
	void Move(vec3 amount);
	void Move(float x,float y,float z);
	void MoveRelative(vec3 amount);
	void MoveRelative(float x, float y, float z);
	void SetPosition(vec3 pos);
	void SetPosition(float x, float y, float z);
	void BuildViewMatrix();
	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);
	void RotateY(float angle);
	void SetLens(float Near, float Far, float aspect_ratio, float fov);
	mat4 GetViewProj();
	mat4 GetView();
	vec3 GetPos();
	vec3 GetLook();
	vec3 GetRight();
	vec3 GetUp();
	float GetFOV();
	void LookAt(vec3 pos);
};