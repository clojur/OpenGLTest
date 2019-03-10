#include "OpenGLObject.h"

#ifdef _WIN32
#pragma comment(linker,"/SUBSYSTEM:CONSOLE")
#endif

#ifdef _WIN32
#define UNUSED_ARGS(...) (void)(true ? (void)0 : ((void)(__VA_ARGS__)))
#else
#define UNUSED_ARGS(...)
#endif

#ifdef USE_NOESISGUI_TEST

#define NOESIS_GUI

#ifdef NOESIS_GUI
#include <NsCore/Noesis.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IView.h>
#include <NsGui/IRenderer.h>
#include <NsGui/FrameworkElement.h>
#include <NsRender/GLFactory.h>
#include <NsApp/EmbeddedXamlProvider.h>
#include <NsApp/EmbeddedFontProvider.h>

#include "resHead/Settings.xaml.bin.h"
#include "resHead/HermeneusOne-Regular.ttf.bin.h"

#endif//NOESIS_GUI

#endif//USE_NOESISGUI_TEST

OpenGLObject::OpenGLObject() :
m_rotate(0.0f),
m_IsFirst(true),
m_iTime(0.0),
m_mouseX(0),
m_mouseY(0)
{

}


OpenGLObject::~OpenGLObject()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteVertexArrays(1, &m_vertexArray);
	glDeleteTextures(1, &m_TextureObject);
}

bool OpenGLObject::InitGlew(CWnd *window)
{
	//������ʱ���ڣ�ֻ��Ϊ�˳�ʼ��glew��
	PIXELFORMATDESCRIPTOR pfd;
	CWnd* tempWnd = new CWnd();
	tempWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 20, 20), window, NULL);
	HDC temHdc = tempWnd->GetDC()->GetSafeHdc();
	//�����������ظ�ʽ
	if (!SetPixelFormat(temHdc, 1, &pfd))//ÿ������ֻ������һ��
		return false;
	HGLRC temphRC = wglCreateContext(temHdc);//����һ����ʱ�Ļ���Ϊ�˳�ʼ��glew,��ʼ������ܹ�ʹ��wglChoosePixelFormatARB��wglCreateContextAttribsARB����
	wglMakeCurrent(temHdc, temphRC);//ֻ�����õ�ǰopengl��������ܹ���ʼ��glew��
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, L"GLEW��ʼ��ʧ�ܣ�", L"��ʾ", MB_ICONERROR);
		return false;
	}
	if (!WGLEW_ARB_create_context || !WGLEW_ARB_pixel_format)
	{
		return false;
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(temphRC);
	tempWnd->DestroyWindow();//������ʱ����
	delete tempWnd;
	return true;
}

bool OpenGLObject::InitializeOpenGL(CDC* pDC)
{
	SetupPixelFormat(pDC);
	::wglMakeCurrent(pDC->GetSafeHdc(), m_hRc);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	return true;
}

bool OpenGLObject::SetupPixelFormat(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat = -1;
	int nPixCount = 0;
	float fPixAttribs[] = { 0, 0 };
	int iPixAttribs[] = {
		WGL_SUPPORT_OPENGL_ARB, 1,//֧��OPENGL��Ⱦ
		WGL_DRAW_TO_WINDOW_ARB, 1,//���ظ�ʽ�������е�����
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,//��HW����
		WGL_COLOR_BITS_ARB, 24,//R,G,Bÿ����ɫ8bit
		WGL_DEPTH_BITS_ARB, 24,//24λ���
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,//˫����������
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,//pf should be RGBA type
		WGL_STENCIL_BITS_ARB, 8,//ģ�滺����8bit
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,//�������ز���
		WGL_SAMPLES_ARB, 4,//���ز�������Ϊ4
		0
	};
	//�µĲ�ѯ���ظ�ʽ�ĺ���
	wglChoosePixelFormatARB(pDC->GetSafeHdc(), iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	//���ز���ʱ�����Ӳ����֧�־�ʹ������Ĵ���رն��ز���
	if (nPixelFormat == -1)
	{
		//try again without MSAA
		iPixAttribs[18] = 1;
		wglChoosePixelFormatARB(pDC->GetSafeHdc(), iPixAttribs, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
	}
	//��ȡһ�����ظ�ʽ������������Ϊ��ǰ��������
	if (!SetPixelFormat(pDC->GetSafeHdc(), nPixelFormat, &pfd))
	{
		MessageBox(NULL, L"�������ظ�ʽʧ�ܣ�", L"��ʾ", MB_ICONERROR);
		return false;
	}
	GLint attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,//���汾3
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,//�ΰ汾3
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//Ҫ�󷵻ؼ���ģʽ����,�����ָ����ָ��ΪWGL_CONTEXT_CORE_PROFILE_BIT_ARB�᷵��ֻ�������Ĺ��ܵĻ���
		0
	};
	m_hRc = wglCreateContextAttribsARB(pDC->GetSafeHdc(), 0, attribs);
	if (m_hRc == NULL)
	{
		MessageBox(NULL, L"���ܴ���OGL3.3 context\n���Դ���3.2��", L"��ʾ", MB_OK);
		attribs[3] = 2;
		m_hRc = wglCreateContextAttribsARB(pDC->GetSafeHdc(), 0, attribs);
		if (m_hRc == NULL)
		{
			MessageBox(NULL, L"opengl3.2 create failed!", L"��ʾ", MB_OK);
			return false;
		}
	}
	wglMakeCurrent(NULL, NULL);
	return true;
}

void OpenGLObject::CreateSceneData()
{
	GLfloat vertices[] = {
		//����				//��������
		-1.0f, -1.0f, 0.0f, 0.0, 0.0,
		1.0f, -1.0f, 0.0f, 1.0, 0.0,
		1.0f, 1.0f, 0.0f, 1.0, 1.0,
		1.0f, 1.0f, 0.0f, 1.0, 1.0,
		-1.0f, 1.0f, 0.0f, 0.0, 1.0,
		-1.0f, -1.0f, 0.0f, 0.0, 0.0
	};
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//CreateTexture(m_TextureObject, GL_REPEAT,GL_NEAREST,GL_NEAREST,"..\\Texture\\ground.png");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	m_Shader.CreateShader();
	std::string vertexShaderCode = m_Shader.GetFileStr("Resource\\ShaderFile\\vertex.glsl");
	std::string fragmentShaderCode = m_Shader.GetFileStr("Resource\\ShaderFile\\ShaderToy_1.glsl");
	m_Shader.AddShader(GL_VERTEX_SHADER, vertexShaderCode);
	m_Shader.AddShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
	m_Shader.LinkShaderProgram();
	m_IsFirst = false;

#ifdef USE_NOESISGUI_TEST
	/*NoesisGUI*/
	auto logHandler = [](const char*, uint32_t, uint32_t level, const char* channel,
		const char* message)
	{
		if (strcmp(channel, "") == 0)
		{
			// [TRACE] [DEBUG] [INFO] [WARNING] [ERROR]
			const char* prefixes[] = { "T", "D", "I", "W", "E" };
			const char* prefix = level < NS_COUNTOF(prefixes) ? prefixes[level] : " ";
			fprintf(stderr, "[NOESIS/%s] %s\n", prefix, message);
		}
	};
	Noesis::GUI::Init(nullptr, logHandler, nullptr);
	NoesisApp::EmbeddedXaml content =
	{
		"Settings.xaml", Settings_xaml, sizeof(Settings_xaml)
	};
	Noesis::GUI::SetXamlProvider(Noesis::MakePtr<NoesisApp::EmbeddedXamlProvider>(&content, 1));
	NoesisApp::EmbeddedFont font =
	{
		"", HermeneusOne_Regular_ttf, sizeof(HermeneusOne_Regular_ttf)
	};

	Noesis::GUI::SetFontProvider(Noesis::MakePtr<NoesisApp::EmbeddedFontProvider>(&font, 1));
	//Noesis::GUI::SetXamlProvider(Noesis::MakePtr<NoesisApp::LocalXamlProvider>("."));
	//Noesis::GUI::SetTextureProvider(Noesis::MakePtr<NoesisApp::LocalTextureProvider>("."));
	//Noesis::GUI::SetFontProvider(Noesis::MakePtr<NoesisApp::LocalFontProvider>("."));
	auto xaml = Noesis::GUI::LoadXaml<Noesis::FrameworkElement>("Settings.xaml");
	_view = Noesis::GUI::CreateView(xaml);
	_view->SetIsPPAAEnabled(true);
	Noesis::Ptr<Noesis::RenderDevice> device = NoesisApp::GLFactory::CreateDevice();
	_view->GetRenderer()->Init(device);
#endif
}

void OpenGLObject::DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#ifndef	USE_NOESISGUI_TEST
	/*Opengl*/
	m_iTime += 0.01f;
	glClearColor(0.2f, 0.5f, 0.4f, 1.0f);
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(m_rotate), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 view;
	view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_Shader.UseProgram();
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureObject);
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "Texture0"), 0);*/
	glViewport(0, 0, m_viewPortW, m_viewPortH);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader.m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(m_Shader.m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
	glUniformMatrix4fv(glGetUniformLocation(m_Shader.m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "viewX"), 0);
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "viewY"), 0);
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "viewW"), m_viewPortW);
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "viewH"), m_viewPortH);
	glUniform1f(glGetUniformLocation(m_Shader.m_shaderProgram, "iTime"), m_iTime);
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "mouseX"), m_mouseX);
	glUniform1i(glGetUniformLocation(m_Shader.m_shaderProgram, "mouseY"), m_mouseY);
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
#endif

#ifdef	USE_NOESISGUI_TEST
	/*NoesisGUI*/
	_view->Update(0.33);
	_view->GetRenderer()->UpdateRenderTree();
	//_view->GetRenderer()->RenderOffscreen();
	//glBindBuffer(GL_FRAMEBUFFER, 0);
	//glClearStencil(0);
	_view->GetRenderer()->Render();
#endif
	
	
	SwapBuffers(wglGetCurrentDC());
}


void OpenGLObject::DestroyOGL()
{
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	if (m_hRc != NULL)
		wglDeleteContext(m_hRc);
	if (m_pDC)
		delete m_pDC;
}

void OpenGLObject::ResetViewPort(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	m_x = x;
	m_y = y;
	m_viewPortW = width;
	m_viewPortH = height;

#ifdef USE_NOESISGUI_TEST
	_view->SetSize(width, height);
#endif
}

void OpenGLObject::CreateTexture(GLuint &texture, GLenum wrapMode, GLenum MAG_filterMode, GLenum MIN_filterMode, const GLchar* path)
{
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_filterMode);
	unsigned char *image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
}

void OpenGLObject::run()
{
	//m_pOGLthread = AfxBeginThread((AFX_THREADPROC)RunOGL, (LPVOID)this);
	std::thread drawThread([this](){
		InitializeOpenGL(m_pDC);
		CreateSceneData();
		DrawScene();
	});
	drawThread.detach();
}