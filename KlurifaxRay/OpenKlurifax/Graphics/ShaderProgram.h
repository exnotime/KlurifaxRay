#pragma once
#include <vector>
#include <map>
#include <stdio.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
class ShaderProgram{

public:
	ShaderProgram(void);
	ShaderProgram(std::vector<Shader*> Shaders);
	~ShaderProgram(void);
	bool Init(std::vector<Shader*> Shaders, bool print);
	GLuint GetHandle();
	void Apply();
	bool Validate();
	GLuint GetUniformLocationByName(const string& name);
	void LoadCompleteShaderProgramFromFile(const string& filename, bool print);
	void SetUniformValue(const string& name, const float value);
	void SetUniformValue(const string& name, const int value);
	void SetUniformValue(const string& name, const unsigned int value);
	void SetUniformValue(const string& name, const bool value);
	void SetUniformValue(const string& name, const glm::vec3& value);
	void SetUniformValue(const string& name, const glm::mat4x4& value);
	void SetUniformValue(const string& name, const glm::vec2& value);
	void SetUniformValue(const string& name, const glm::vec4& value);
	void SetUniformTexture(const string& name,Texture& tex);
	GLint FetchUniform(const string& name);
	bool Recompile();
private:
	GLuint m_Handle;
	std::vector<Shader*> m_Shaders;
	bool m_LoadedShaders;
	map<string, GLint> m_UniformMap;
	int m_TextureCount;
	bool m_Validated = false;
	string m_Filename;

	void CreateShader(ifstream* FileStream, GLenum shaderType, const string& filename, bool print);
	std::string GetDir(std::string filename);
	std::string LoadText(const char* filename);
};

