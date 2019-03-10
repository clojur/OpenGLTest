#include "Shader.h"


CShader::CShader()
{
}


CShader::~CShader()
{
	glDeleteProgram(m_shaderProgram);
}

std::string CShader::GetFileStr(const char* fileName)
{
	std::fstream file;
	std::stringstream fileStream;
	file.exceptions(std::fstream::badbit);
	try
	{
		file.open(fileName);
		fileStream << file.rdbuf();
		file.close();
	}
	catch (std::ifstream::failure e)
	{
		TRACE("%s\n",_T("shader File Read Failed!\n"));
		MessageBox(NULL, _T("shaderRes"), _T("Read file path failed!"), MB_ICONERROR);
	}
	auto str = fileStream.str();
	return str;
}

void CShader::AddShader(GLenum shaderType, std::string shaderSource)
{
	GLuint shader;
	GLint success;
	const char* code = shaderSource.c_str();
	GLchar info[512];
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &code, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
		std::string str("ERROR::SHADER::COMPILATION::FAILED\n");
		str.append(info);
		TRACE("%s\n", str.c_str());
		MessageBox(NULL, (LPCWSTR)str.c_str(), L"着色器", MB_ICONERROR);
	}
	glAttachShader(m_shaderProgram, shader);
	glDeleteShader(shader);
}

void CShader::LinkShaderProgram()
{
	GLint success;
	GLchar info[512];
	glLinkProgram(m_shaderProgram);
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, sizeof(info), nullptr, info);
		std::string str("ERROR::PROGRAM::LINK::FAILED\n");
		str.append(info);
		TRACE("%s\n", str.c_str());
		MessageBox(NULL, (LPCWSTR)str.c_str(), L"着色器程序", MB_ICONERROR);
	}
}

void CShader::UseProgram()
{
	glUseProgram(m_shaderProgram);
}

void CShader::CreateShader()
{
	m_shaderProgram = glCreateProgram();
}
