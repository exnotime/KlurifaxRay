#include "Camera.h"

Camera::Camera()
{
	m_View			=	mat4(1.0f);
	m_Projection	=	mat4(1.0f);
	m_Position		=	vec3(0.0f);
	m_Right			=	vec3(1.0f,0,0);
	m_Up			=	vec3(0,1.0f,0);
	m_Look			=	vec3(0,0,1.0f);
	BuildViewMatrix();
}

Camera::~Camera()
{

}

void Camera::Move(vec3 amount)
{
	m_Position += amount;
	BuildViewMatrix();
}

void Camera::MoveRelative(vec3 amount)
{
	vec3 movement;
	movement += m_Right * amount.x;
	movement += m_Up * amount.y;
	movement += m_Look * amount.z;
	m_Position += movement;
	BuildViewMatrix();
}

void Camera::Move(float x,float y,float z)
{
	m_Position += vec3(x,y,z);
	BuildViewMatrix();
}

void Camera::MoveRelative(float x, float y, float z)
{
	vec3 movement;
	movement += m_Right * x;
	movement += m_Up * y;
	movement += m_Look * z;
	m_Position += movement;
	BuildViewMatrix();
}

void Camera::SetPosition(vec3 pos)
{
	m_Position = pos;
	BuildViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position = vec3(x,y,z);
	BuildViewMatrix();
}

void Camera::Roll(float angle)
{
	mat4 R = glm::rotate(angle,m_Look);
	vec4 r = (vec4(m_Right,0) * R);
	m_Right.x = r.x;
	m_Right.y = r.y;
	m_Right.z = r.z;
	vec4 u = (vec4(m_Up,0) * R);
	m_Up.x = u.x;
	m_Up.y = u.y;
	m_Up.z = u.z;
	BuildViewMatrix();
}

void Camera::Pitch(float angle)
{
	mat4 R = glm::rotate(angle,m_Right);
	vec4 l = (vec4(m_Look,0) * R);
	m_Look.x = l.x;
	m_Look.y = l.y;
	m_Look.z = l.z;
	vec4 u = (vec4(m_Up,0) * R);
	m_Up.x = u.x;
	m_Up.y = u.y;
	m_Up.z = u.z;
	BuildViewMatrix();
}

void Camera::Yaw(float angle)
{
	mat4 R = glm::rotate(angle,m_Up);
	vec4 l = (vec4(m_Look,0) * R);
	m_Look.x = l.x;
	m_Look.y = l.y;
	m_Look.z = l.z;
	vec4 r = (vec4(m_Right,0) * R);
	m_Right.x = r.x;
	m_Right.y = r.y;
	m_Right.z = r.z;
	BuildViewMatrix();
}

void Camera::RotateY(float angle)
{
	mat4 R = glm::rotate(angle,vec3(0,1,0));
	vec4 l = (vec4(m_Look,0) * R);
	m_Look.x = l.x;
	m_Look.y = l.y;
	m_Look.z = l.z;
	vec4 r = (vec4(m_Right,0) * R);
	m_Right.x = r.x;
	m_Right.y = r.y;
	m_Right.z = r.z;
	vec4 u = (vec4(m_Up,0) * R);
	m_Up.x = u.x;
	m_Up.y = u.y;
	m_Up.z = u.z;
	BuildViewMatrix();
}

void Camera::SetLens(float Near, float Far, float aspect_ratio, float fov)
{
	m_Projection = glm::perspective(fov,aspect_ratio,Near,Far);
	m_Fov = fov;
}

void Camera::BuildViewMatrix()
{
	vec3 R = m_Right;
	vec3 L = m_Look;
	vec3 U = m_Up;

 	L = glm::normalize(L);
	vec3 temp = cross(L, R);
	U = normalize(temp);
	R = cross(U, L);

	float x = -dot(m_Position, R);
	float y = -dot(m_Position, U);
	float z = -dot(m_Position, L);

	m_Right = R;
	m_Look = L;
	m_Up = U;

	m_View[0][0] = m_Right.x;
	m_View[1][0] = m_Right.y;
	m_View[2][0] = m_Right.z;
	m_View[3][0] = x;

	m_View[0][1] = m_Up.x;
	m_View[1][1] = m_Up.y;
	m_View[2][1] = m_Up.z;
	m_View[3][1] = y;

	m_View[0][2] = m_Look.x;
	m_View[1][2] = m_Look.y;
	m_View[2][2] = m_Look.z;
	m_View[3][2] = z;

	m_View[0][3] = 0;
	m_View[1][3] = 0;
	m_View[2][3] = 0;
	m_View[3][3] = 1;
}

mat4 Camera::GetViewProj()
{
	return m_Projection * m_View;
}
mat4 Camera::GetView(){
	return m_View;
}

vec3 Camera::GetPos()
{
	return m_Position;
}

vec3 Camera::GetLook()
{
	return m_Look;
}
vec3 Camera::GetRight()
{
	return m_Right;
}
vec3 Camera::GetUp()
{
	return m_Up;
}

float Camera::GetFOV(){
	return m_Fov;
}
void Camera::LookAt(vec3 pos){
	m_Look = normalize(m_Position - pos);
	m_Right = normalize(cross(vec3(0, 1, 0), m_Look));
	m_Up = normalize(cross(m_Look, m_Right));
}
