#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(void){
	m_LoadedShaders = false;
}

ShaderProgram::ShaderProgram(std::vector<Shader*> Shaders){
	this->m_Shaders = Shaders;
	Init(m_Shaders,true);
}

ShaderProgram::~ShaderProgram(void)
{
	if (m_LoadedShaders){
		for (std::vector<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it){
			delete *it;
		}
		m_Shaders.clear();
	}
	
	glDeleteProgram(m_Handle);
}

bool ShaderProgram::Init(std::vector<Shader*> Shaders, bool print){
	if (m_Shaders.size() > 0){
		m_Shaders.clear();
	}
	//save shaders
	for (auto it : Shaders){
		m_Shaders.push_back(it);
	}
	m_Handle = glCreateProgram();
	if(m_Handle == 0){
		throw "Failed to create shader program";
		return false;
	}
	//attach shaders to program
	for(int i = 0; i < static_cast<int>(Shaders.size()); i++) {
		glAttachShader(m_Handle,Shaders[i]->GetHandle());
	}
	
	//link shaders
	glLinkProgram(m_Handle);

	for (int i = 0; i < static_cast<int>(Shaders.size()); i++){
		glDetachShader(m_Handle, Shaders[i]->GetHandle());
		Shaders[i]->Clear();
	}
	//error checking
	GLint status;
	glGetProgramiv(m_Handle,GL_LINK_STATUS,&status);
	if(status == GL_FALSE) {
		char log[1024];
		int len = 0;
		printf("Cant link shaders \n");
		glGetProgramInfoLog(m_Handle, sizeof(log), &len, log);
		printf("Errorlog : %s \n", log);
		return false;
	}
	if (print)
		printf("linked Shaderprogram \n");
		
	// Not using "return Validate();" because stuff might get added after
	if (!Validate()) {
		return false;
	}
	return true;
}

GLuint ShaderProgram::GetHandle(){
	return m_Handle;
}

void ShaderProgram::Apply() {
	// Lazy validation because of AMD driver behaviour.
	if (!m_Validated) {
		if (!Validate()) {
			GLint status;
			glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &status);
			if ( status == GL_FALSE ) {
				char log[1024];
				int len = 0;
				glGetProgramInfoLog( m_Handle, sizeof( log ), &len, log );
				printf(("Validation of shader program failed: " + string(log, len - 1)).c_str());
				// TODO assert or fancy recovery when recompiling in run-time? assert for now.
				assert(false);
			}
		}
	}

	m_TextureCount = 0;
	glUseProgram(m_Handle);
}

bool ShaderProgram::Validate() {
	glValidateProgram(m_Handle);
	GLint status;
	glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &status);
	m_Validated = (status == GL_FALSE);
	return m_Validated;
}

GLuint ShaderProgram::GetUniformLocationByName(const string& name){
	return glGetUniformLocation(m_Handle,name.c_str());
}

void ShaderProgram::LoadCompleteShaderProgramFromFile(const string& filename, bool print){
	//this function is used to load a set of shaders from a single file
	//seperate each shader with a #new_shader ******* and a #end_shader
	//where **** can be a vertex, geometry, control, evaluation or fragment
	//extra code can be loaded into the current shader by the #include filename macro
	m_Filename = string(filename);
	m_LoadedShaders = true;
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open()){
		if (print)
			printf("Could not open file %s\n", filename.c_str());
		return;
	}
	string line;
	size_t found;
	while (!fin.eof()){
		getline(fin, line);
		if ((found = line.find("#new_shader")) != std::string::npos){
			if ((found = line.find("vertex")) != std::string::npos){
				CreateShader(&fin, GL_VERTEX_SHADER, filename,print);
			}
			if ((found = line.find("geometry")) != std::string::npos){
				CreateShader(&fin, GL_GEOMETRY_SHADER, filename, print);
			}
			if ((found = line.find("control")) != std::string::npos){
				CreateShader(&fin, GL_TESS_CONTROL_SHADER, filename, print);
			}
			if ((found = line.find("evaluation")) != std::string::npos){
				CreateShader(&fin, GL_TESS_EVALUATION_SHADER, filename, print);
			}
			if ((found = line.find("fragment")) != std::string::npos){
				CreateShader(&fin, GL_FRAGMENT_SHADER, filename, print);
			}
			if ((found = line.find("compute")) != std::string::npos){
				CreateShader(&fin, GL_COMPUTE_SHADER, filename, print);
			}
		}
	}

	m_Handle = glCreateProgram();
	
	if (m_Handle == 0){
		if (print)
			printf("Could not create shader program\n");
		return;
	}
	//attach shaders to program
	for (int i = 0; i < static_cast<int>(m_Shaders.size()); i++){
		glAttachShader(m_Handle, m_Shaders[i]->GetHandle());
		
	}
	
	//link shaders
	glLinkProgram(m_Handle);
	//clear shaders
	for (int i = 0; i < static_cast<int>(m_Shaders.size()); i++){
		glDetachShader(m_Handle, m_Shaders[i]->GetHandle());
		m_Shaders[i]->Clear();
	}
	//error checking
	GLint status = 0;
	glGetProgramiv(m_Handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE){
		char log[1024];
		int len = 0;
		printf("Could not link shaders to program\n");
		glGetProgramInfoLog(m_Handle, sizeof(log), &len, log);
		printf("Error log : %s\n", log);
		return;
	}
	if (print)
		printf("Linked Shaders to program\n");
	Validate();
}
//Creates a shader
void ShaderProgram::CreateShader(ifstream* FileStream, GLenum shaderType, const string& filename,bool print){
	string line;
	size_t found;

	string shaderText;
	//load a vertex shader
	bool end = false;
	while (!end){
		getline(*FileStream, line);
		//search for the end of the shader
		found = line.find("#include ");
		if (found != std::string::npos){
			int i = static_cast<int>(found) + 9; //found + "#include "
			string s;
			while (i < static_cast<int>(line.length())){
				s += line[i];
				i++;
			}
			string str = GetDir(string(filename)) + s;
			shaderText += LoadText(str.c_str());
			shaderText += "\n";
		}
		else{
			found = line.find("#end_shader");
			if (found != std::string::npos){
				//break loop
				end = true;
				break;
			}
			else if (FileStream->eof()){
				printf("Could not find end of file %s\n", filename.c_str());
				return;
			}
			else{
				shaderText += line;
				shaderText += '\n';
			}
			
		}

	}
	if (shaderText.length() > 0){
		Shader* shader = new Shader();
		shader->CreateFromString(shaderText, shaderType, filename,print);
		m_Shaders.push_back(shader);
		shaderText.clear();
	}
}

std::string ShaderProgram::GetDir(std::string filename){
	bool found = false;
	for (int i = static_cast<int>(filename.size()); i > 0; i--){
		if (filename.c_str()[i] == '/')
			found = true;
		if (!found){
			filename.erase(i);
		}

	}
	return filename;
}

std::string ShaderProgram::LoadText(const char* filename){
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open())
		return "";
	string text;
	string line;
	while (!fin.eof()){
		getline(fin, line);
		text += line + "\n";
	}
	return text;

}

void ShaderProgram::SetUniformValue(const string& name, const float value){
	glUniform1f(FetchUniform(name), value);
}

void ShaderProgram::SetUniformValue(const string& name, const int value){
	glUniform1i(FetchUniform(name), value);
}

void ShaderProgram::SetUniformValue(const string& name, const unsigned int value){
	glUniform1ui(FetchUniform(name), value);
}

void ShaderProgram::SetUniformValue(const string& name, const bool value){
	glUniform1i(FetchUniform(name), value);
}

void ShaderProgram::SetUniformValue(const string& name, const glm::vec3& value){
	glUniform3f(FetchUniform(name), value.x,value.y,value.z);
}

void ShaderProgram::SetUniformValue(const string& name, const glm::vec2& value){
	glUniform2f(FetchUniform(name), value.x, value.y);
}

void ShaderProgram::SetUniformValue(const string& name, const glm::vec4& value){
	glUniform4f(FetchUniform(name), value.x, value.y, value.z,value.w);
}

void ShaderProgram::SetUniformValue(const string& name, const glm::mat4x4& value){
	glUniformMatrix4fv(FetchUniform(name), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::SetUniformTexture(const string& name, Texture& tex){
	tex.Apply(FetchUniform(name), m_TextureCount++);
}

GLint ShaderProgram::FetchUniform(const string& name){
	map<string, GLint>::const_iterator it = m_UniformMap.find(name);
	// Uniform wasn't found in cache
	if (it == m_UniformMap.end()) {
		GLint loc = GetUniformLocationByName(name);
		// Don't emplace in cache if uniform was invalid
		if (loc != -1) {
			m_UniformMap.emplace(name, loc);
		} else {
			//printf("Failed to fetch unifrom : %s\n",name.c_str());
			//assert(false); //makes it annoying to debug shaders
		}
		return loc;
	// Uniform was found in cache
	} else {
		return it->second;
	}
}

bool ShaderProgram::Recompile(){
	if (!m_LoadedShaders){
		for (auto it : m_Shaders){
			it->Recompile();
		}
		glDeleteProgram(m_Handle);
		m_Validated = false;
		return Init(m_Shaders, false);;
	} else {
		//clear up old shader
		for (std::vector<Shader*>::iterator i = m_Shaders.begin(); i != m_Shaders.end(); i++){
			delete  *i;
		}
		m_Shaders.clear();
		glDeleteProgram(m_Handle);
		//reinit the program
		LoadCompleteShaderProgramFromFile(m_Filename, false);
		m_Validated = false;
		return true;
	}

}