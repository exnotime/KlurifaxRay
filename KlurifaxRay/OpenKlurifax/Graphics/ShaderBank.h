#pragma once
#include <map>
#include <string>
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderBank.h"

using std::map;
using std::vector;
using std::string;

#define g_ShaderBank ShaderBank::GetInstance()

struct ShaderProgramEntry {
	ShaderProgramEntry(const string& path, int type) :
	Path(path), Type(type) {

	}

	string Path;
	int Type;
};

class ShaderBank{
public:
	static ShaderBank* GetInstance();
	~ShaderBank();

	ShaderProgram* CreateProgram(const vector<ShaderProgramEntry>& shaders, const string& name);
	ShaderProgram* CreateCompleteProgram(const string& filename, const string& name);
	ShaderProgram* GetProgram(const string& name);

	void RecompileAll();
	void Recompile(const string& name);

	void ClearShaders();
private:
	Shader* LoadShader(int type, const string& path, bool reload = false);

	map<string, Shader*> m_shaderMap;
	map<string, ShaderProgram*> m_shaderProgramMap;
};
