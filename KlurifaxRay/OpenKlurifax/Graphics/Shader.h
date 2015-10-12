#pragma once
#include <GL/glew.h>
#include <fstream>
#include <stdio.h>
#include <string>
using namespace std;
class Shader
{
private:
	GLuint m_Handle;
	string m_Path;
	GLenum m_Type;
public:
	Shader();
	~Shader();
	bool CreateFromFile(const string& Filename,GLenum ShaderType, bool print);
	bool CreateFromString(string ShaderCode, GLenum ShaderType, const string& filename, bool print);
	GLuint  GetHandle();
	std::string GetDir(std::string filename);
	std::string LoadText(const string& filename);
	void Clear();
	void Recompile();
};