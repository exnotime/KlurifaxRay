#include "ShaderBank.h"
#include "../Utility/MemoryManagement.h"

ShaderBank* ShaderBank::GetInstance()
{
	static ShaderBank input;
	return &input;
}

ShaderBank::~ShaderBank() {
	DeleteMapPointers(m_shaderProgramMap);
	DeleteMapPointers(m_shaderMap);
}

Shader* ShaderBank::LoadShader(int type, const string& path, bool reload) {
	// Try to find in map
	map<string, Shader*>::iterator it = m_shaderMap.find(path);

	// Force to load the shader again if reload is true
	if (reload) {
		// Only delete existing
		if (it != m_shaderMap.end()) {
			SAFE_DELETE(it->second);
			it = m_shaderMap.end();
		}
	}

	// Already loaded, get from map
	if (it != m_shaderMap.end()) {
		return it->second;
	}
	// Not loaded; load it and emplace in map
	else {
		Shader* shader = new Shader();
		shader->CreateFromFile(path.c_str(),type,true);
		m_shaderMap.emplace(path, shader);
		return shader;
	}
}

ShaderProgram* ShaderBank::CreateProgram(const vector<ShaderProgramEntry>& shaderEntries, const string& name) {
	// Check if program already exist
	if (m_shaderProgramMap.find(name) != m_shaderProgramMap.end())
		throw "Shaderprogram already exist: " + name;

	vector<Shader*> shaders;
	// Load all shaders in the shader program entry list
	for (vector<ShaderProgramEntry>::const_iterator it = shaderEntries.begin(); it != shaderEntries.end(); ++it) {
		shaders.push_back(LoadShader(it->Type, it->Path));
	}
	// If any shaders were pushed back, make a shader program out of them and emplace it in the shader program map
	if (shaders.size() > 0) {
		ShaderProgram* shaderProgram = new ShaderProgram(shaders);
		m_shaderProgramMap.emplace(name, shaderProgram);
		return shaderProgram;
	}
	else
		throw "Shader program needs shaders connected to it";
}

ShaderProgram* ShaderBank::CreateCompleteProgram(const string& filename, const string& name){
	// Check if program already exist
	if (m_shaderProgramMap.find(name) != m_shaderProgramMap.end())
		throw "Shaderprogram already exist: " + name;
	ShaderProgram* shaderprog = new ShaderProgram();
	shaderprog->LoadCompleteShaderProgramFromFile(filename,true);
	m_shaderProgramMap.emplace(name, shaderprog);
	return shaderprog;
}

ShaderProgram* ShaderBank::GetProgram(const string& name) {
	map<string, ShaderProgram*>::iterator it = m_shaderProgramMap.find(name);
	return it == m_shaderProgramMap.end() ? nullptr : it->second;
}

void ShaderBank::RecompileAll(){
	for (auto it : m_shaderProgramMap){
		it.second->Recompile();
	}
}

void ShaderBank::Recompile(const string& name){
	auto it = m_shaderProgramMap.find(name);
	if (it != m_shaderProgramMap.end()){
		it->second->Recompile();
	}
	else{
		throw "Shaderprogram " + name + "does not exist";
	}
	
}