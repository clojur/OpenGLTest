#pragma once
#include "common.h"
#include "Shader.h"
class MyOpenGL_API OpenGLObject
{
public:
	OpenGLObject();
	~OpenGLObject();
	bool InitGlew(CWnd* window);
	bool InitializeOpenGL(CDC* pDC);
	bool SetupPixelFormat(CDC* pDC);
	void DrawScene();
	void CreateSceneData();
	void DestroyOGL();
#ifdef USE_NOESISGUI_TEST
	Noesis::Ptr<Noesis::IView> GetXMLUI() { return _view; }
#endif
	void ResetViewPort(int x,int y,int width,int height);
	void CreateTexture(GLuint &texture, GLenum wrapMode, GLenum MAG_filterMode, GLenum MIN_filterMode, const GLchar* path);
	void run();

public:
	CClientDC	*m_pDC;
	CRect	m_oldRect;
	HGLRC	m_hRc;
	GLuint m_vertexArray;
	GLuint m_vertexBuffer;
	GLuint m_TextureObject;
	CShader m_Shader;
	bool m_IsFirst;

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;
	float m_rotate;
public:
	int m_x;
	int m_y;
	int m_viewPortW;
	int m_viewPortH;
	int m_mouseX;
	int m_mouseY;
	GLfloat m_iTime;
private:
	CWinThread *m_pOGLthread;
/*noesisGUI*/
#ifdef USE_NOESISGUI_TEST
public:
	Noesis::Ptr<Noesis::FrameworkElement> _context;
	Noesis::Ptr<Noesis::IView>			_view;
#endif()
};

