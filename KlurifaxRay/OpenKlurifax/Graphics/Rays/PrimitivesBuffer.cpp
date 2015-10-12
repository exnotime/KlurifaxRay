#include "PrimitivesBuffer.h"

PrimitivesBuffer::PrimitivesBuffer(){

}

PrimitivesBuffer::~PrimitivesBuffer(){
	glDeleteBuffers(1, &m_Buffer);
}

void PrimitivesBuffer::Init(){
	m_Spheres.reserve(MaxSpheres);
	m_Triangles.reserve(MaxTriangles);

	glGenBuffers(1, &m_Buffer);
	if (m_Buffer <= 0){
		printf("could not create Prmitives buffer\n");
		return;
	}
}

void PrimitivesBuffer::Build(){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Buffer);
	//create buffer
	unsigned char* buffer = new unsigned char[BufferSize];
	memcpy(buffer, m_Spheres.data(), m_Spheres.size() * sizeof(Sphere));//copy spheres
	memcpy((void*)(buffer + TrianglesBufferPos), m_Triangles.data(), m_Triangles.size() * sizeof(Triangle)); //copy triangles
	glBufferData(GL_SHADER_STORAGE_BUFFER, BufferSize,buffer, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	delete[] buffer;
}

void PrimitivesBuffer::Bind(GLuint program, GLuint blockBinding){
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Buffer);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, blockBinding, m_Buffer, (GLintptr)0, (GLsizeiptr)BufferSize);
	GLuint blockIndex = 0;
	blockIndex = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "PrimitiveBuffer");
	glShaderStorageBlockBinding(program, blockIndex, blockBinding);
}

GLuint PrimitivesBuffer::GetBuffer(){
	return m_Buffer;
}

void PrimitivesBuffer::AddSphere(Sphere s){
	m_Spheres.push_back(s);
}

void PrimitivesBuffer::AddTriangle(Triangle t){
	m_Triangles.push_back(t);
}

int PrimitivesBuffer::nSpheres(){
	return (int)m_Spheres.size();
}
int PrimitivesBuffer::nTriangles(){
	return (int)m_Triangles.size();
}

void PrimitivesBuffer::GenerateAABB(vec3 minBounds, vec3 maxBounds){
	Triangle t;
	t.Color = vec4(1.0f, 1.0f, 1.0f,1.0f);

	float sizeX, sizeY, sizeZ;

	sizeX = maxBounds.x - minBounds.x;
	sizeY = maxBounds.y - minBounds.y;
	sizeZ = maxBounds.z - minBounds.z;
	//-y
	t.p0 = vec4(minBounds.x, minBounds.y, minBounds.z, 1.0f);
	t.p2 = vec4(minBounds.x,minBounds.y,minBounds.z + sizeZ, 1.0f);
	t.p1 = vec4(minBounds.x + sizeX, minBounds.y, minBounds.z, 1.0f);
	m_Triangles.push_back(t);
	t.p0 = vec4(minBounds.x, minBounds.y, minBounds.z + sizeZ, 1.0f);
	t.p2 = vec4(minBounds.x + sizeX, minBounds.y, minBounds.z + sizeZ, 1.0f);
	t.p1 = vec4(minBounds.x + sizeX, minBounds.y, minBounds.z, 1.0f);
	m_Triangles.push_back(t);
	//+y
	t.p0 = vec4(maxBounds.x, maxBounds.y, maxBounds.z, 1.0f);
	t.p1 = vec4(maxBounds.x, maxBounds.y, maxBounds.z - sizeZ, 1.0f);
	t.p2 = vec4(maxBounds.x - sizeX, maxBounds.y, maxBounds.z, 1.0f);
	m_Triangles.push_back(t);
	t.p0 = vec4(maxBounds.x, maxBounds.y, maxBounds.z - sizeZ, 1.0f);
	t.p1 = vec4(maxBounds.x - sizeX, maxBounds.y, maxBounds.z - sizeZ, 1.0f);
	t.p2 = vec4(maxBounds.x - sizeX, maxBounds.y, maxBounds.z, 1.0f);
	m_Triangles.push_back(t);
	//-x
	t.p0 = vec4(minBounds.x, minBounds.y, minBounds.z, 1.0f);
	t.p2 = vec4(minBounds.x, minBounds.y + sizeY, minBounds.z, 1.0f);
	t.p1 = vec4(minBounds.x, minBounds.y, minBounds.z + sizeZ, 1.0f);
	m_Triangles.push_back(t);
	t.p0 = vec4(minBounds.x, minBounds.y + sizeY, minBounds.z, 1.0f);
	t.p2 = vec4(minBounds.x, minBounds.y + sizeY, minBounds.z + sizeZ, 1.0f);
	t.p1 = vec4(minBounds.x, minBounds.y, minBounds.z + sizeZ, 1.0f);
	m_Triangles.push_back(t);
	//+x
	t.p0 = vec4(maxBounds.x, maxBounds.y, maxBounds.z, 1.0f);
	t.p1 = vec4(maxBounds.x, maxBounds.y - sizeY, maxBounds.z, 1.0f);
	t.p2 = vec4(maxBounds.x, maxBounds.y, maxBounds.z - sizeZ, 1.0f);
	m_Triangles.push_back(t);
	t.p0 = vec4(maxBounds.x, maxBounds.y - sizeY, maxBounds.z, 1.0f);
	t.p1 = vec4(maxBounds.x, maxBounds.y - sizeY, maxBounds.z - sizeZ, 1.0f);
	t.p2 = vec4(maxBounds.x, maxBounds.y, maxBounds.z - sizeZ, 1.0f);
	m_Triangles.push_back(t);
	//-z
	t.p0 = vec4(minBounds.x, minBounds.y, minBounds.z, 1.0f);
	t.p1 = vec4(minBounds.x, minBounds.y + sizeY, minBounds.z, 1.0f);
	t.p2 = vec4(minBounds.x + sizeX, minBounds.y, minBounds.z, 1.0f);
	m_Triangles.push_back(t);
	t.p0 = vec4(minBounds.x, minBounds.y + sizeY, minBounds.z, 1.0f);
	t.p1 = vec4(minBounds.x + sizeX, minBounds.y + sizeY, minBounds.z, 1.0f);
	t.p2 = vec4(minBounds.x + sizeX, minBounds.y, minBounds.z, 1.0f);
	m_Triangles.push_back(t);
	//+z
	t.p0 = vec4(maxBounds.x, maxBounds.y, maxBounds.z, 1.0f);
	t.p2 = vec4(maxBounds.x, maxBounds.y - sizeY, maxBounds.z, 1.0f);
	t.p1 = vec4(maxBounds.x - sizeX, maxBounds.y, maxBounds.z, 1.0f);
	m_Triangles.push_back(t);
	t.p0 = vec4(maxBounds.x, maxBounds.y - sizeY, maxBounds.z, 1.0f);
	t.p2 = vec4(maxBounds.x - sizeX, maxBounds.y - sizeY, maxBounds.z, 1.0f);
	t.p1 = vec4(maxBounds.x - sizeX, maxBounds.y, maxBounds.z, 1.0f);
	m_Triangles.push_back(t);
}