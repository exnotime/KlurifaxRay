#include "Shader.h"
Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteShader(m_Handle);
}

bool Shader::CreateFromFile(const string& Filename, GLenum ShaderType, bool print)
{
	m_Path = Filename;
	m_Type = ShaderType;
	ifstream fin;
	fin.open(Filename);
	if(!fin.is_open())
	{
		if (print)
			printf("Cant open shader file %s\n", Filename.c_str());
		return false;
	}
	string shadertext;
	size_t found;
	string line;
	while(!fin.eof())
	{
		getline(fin, line);
		found = line.find("#include ");
		if (found != std::string::npos)
		{
			int i = static_cast<int>(found) + 9; //found + "#include "
			string s;
			while (i < line.length())
			{
				s += line[i];
				i++;
			}
			string str = GetDir(string(Filename)) + s;
			shadertext += LoadText(str.c_str());
			shadertext += "\n";
		}
		else
		{
			shadertext += line;
			shadertext += '\n';
		}
	}
	fin.close();

	//create shader object
	m_Handle = glCreateShader(ShaderType);
	if(m_Handle == 0)
	{
		if (print)
			printf("Error creating shader\n");
	}
	const char* text = shadertext.c_str();
	glShaderSource(m_Handle,1,&text,nullptr);

	glCompileShader(m_Handle);
	int result = 0;
	glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		if (print)
			printf("ERROR, compiling shader%s\n", Filename.c_str());
		int length = 0;
		glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &length);
		if(length > 0)
		{
			// create a log of error messages
			char* errorLog = new char[length];
			int written = 0;

			glGetShaderInfoLog(m_Handle, length, &written, errorLog);
			if (print)
				printf("Error log: %s \n", errorLog);
			delete [] errorLog;
		}
		return false;
	}
	else
	{
		if (print)
			printf("Compiled Shader %s \n",Filename.c_str());
	}
	return true;
}

bool Shader::CreateFromString(string ShaderCode, GLenum ShaderType, const string& filename, bool print)
{
	//create shader object
	m_Handle = glCreateShader(ShaderType);
	if (m_Handle == 0)
	{
		if (print)
			printf("Error creating shader\n");
	}
	const char* text = ShaderCode.c_str();
	glShaderSource(m_Handle, 1, &text, nullptr);

	glCompileShader(m_Handle);
	int result = 0;
	glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		if (print)
			printf("ERROR, compiling shader%s\n", filename.c_str());
		int length = 0;
		glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			// create a log of error messages
			char* errorLog = new char[length];
			int written = 0;
			glGetShaderInfoLog(m_Handle, length, &written, errorLog);
			if (print)
				printf("Error log: %s \n", errorLog);
			delete[] errorLog;
		}
		return false;
	}
	else
	{
		m_Type = ShaderType;
		m_Path = filename;
		if (print)
			printf("Compiled Shader %s \n", filename.c_str());
	}
	return true;
}

GLuint Shader::GetHandle()
{
	return m_Handle;
}

std::string Shader::GetDir(std::string filename)
{
	bool found = false;
	for (int i = static_cast<int>(filename.size()); i > 0; i--)
	{
		if (filename.c_str()[i] == '/')
			found = true;
		if (!found)
		{
			filename.erase(i);
		}

	}
	return filename;
}

std::string Shader::LoadText(const string&filename)
{
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open())
		return "";
	std::string text;
	string line;
	while (!fin.eof())
	{
		getline(fin, line);
		text += line;
	}
	return text;
}

void Shader::Clear(){
	//when a shader has been linked to a program  there is no longer any need to save the shader(unless you want to reuse the shader)
	//so we clear shaders after we are done with them to free up memory.
	glDeleteShader(m_Handle);
}

void Shader::Recompile(){
	Clear();
	CreateFromFile(m_Path, m_Type, false);
}