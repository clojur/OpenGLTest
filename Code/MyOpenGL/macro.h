#pragma once

#ifdef MyOpenGL_DLL
#define MyOpenGL_API __declspec(dllexport)
#else         
#define MyOpenGL_API  __declspec(dllimport)  
#ifdef _DEBUG
#pragma comment(lib,"MyOpenGLd.lib")
#pragma message("Automatically linking with MyOpenGLd.dll")
#else
#pragma comment(lib,"RouteCross.lib")
#pragma message("Automatically linking with MyOpenGL.dll")
#endif
#endif