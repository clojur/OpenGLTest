#pragma once
#include "common.h"
#include <iostream>
#include <fstream>
#include<sstream>
class MyOpenGL_API CShader
{
public:
	CShader();
	~CShader();
	std::string GetFileStr(const char* fileName);
	void CreateShader();
	void AddShader(GLenum shaderType, std::string shaderSource);
	void LinkShaderProgram();
	void UseProgram();
public:
	GLuint m_shaderProgram;
};
