// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// OGLGUIView.cpp : COGLGUIView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OGLGUI.h"
#endif

#include "OGLGUIDoc.h"
#include "OGLGUIView.h"
#include <process.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COGLGUIView

IMPLEMENT_DYNCREATE(COGLGUIView, CView)

BEGIN_MESSAGE_MAP(COGLGUIView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// COGLGUIView 构造/析构

COGLGUIView::COGLGUIView()
{
	// TODO:  在此处添加构造代码

}

COGLGUIView::~COGLGUIView()
{
}

BOOL COGLGUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// COGLGUIView 绘制

void COGLGUIView::OnDraw(CDC* /*pDC*/)
{
	COGLGUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_OGL->DrawScene();
	// TODO:  在此处为本机数据添加绘制代码
}

void COGLGUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COGLGUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COGLGUIView 诊断

#ifdef _DEBUG
void COGLGUIView::AssertValid() const
{
	CView::AssertValid();
}

void COGLGUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COGLGUIDoc* COGLGUIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COGLGUIDoc)));
	return (COGLGUIDoc*)m_pDocument;
}
#endif //_DEBUG


// COGLGUIView 消息处理程序


int COGLGUIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Toolbar.Create(this);//创建工具栏控件
	m_Toolbar.LoadToolBar(IDR_MAINFRAME);//装载工具栏资源
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_Toolbar.ShowWindow(SW_SHOW);//显示工具栏

	// TODO:  在此添加您专用的创建代码
	m_OGL = new OpenGLObject;
	if (m_OGL->InitGlew(this))
	{
		m_OGL->m_pDC = new CClientDC(this);
		m_OGL->InitializeOpenGL(m_OGL->m_pDC);
		m_OGL->CreateSceneData();
		GetClientRect(&m_OGL->m_oldRect);
	}
	//int iTime=1000.0*(1.0/60.0)
	SetTimer(1,20, NULL);
	return 0;
}


void COGLGUIView::OnDestroy()
{
	CView::OnDestroy();
	// TODO:  在此处添加消息处理程序代码
	m_OGL->DestroyOGL();
}


BOOL COGLGUIView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return true;
}


void COGLGUIView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (cy > 0)
	{
		if (m_OGL->m_oldRect.right > cx || m_OGL->m_oldRect.bottom > cy)
			RedrawWindow();
		m_OGL->m_oldRect.right = cx;
		m_OGL->m_oldRect.bottom = cy;
		m_OGL->ResetViewPort(0, 0, cx, cy);
		m_OGL->m_projection = glm::perspective(45.0f, (float)cx / (float)cy, 0.01f, 100.0f);
	}
	// TODO:  在此处添加消息处理程序代码
}


void COGLGUIView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void COGLGUIView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO:  在此添加专用代码和/或调用基类
	//m_OGL->run();
}


void COGLGUIView::OnMove(int x, int y)
{
	CView::OnMove(x, y);
	// TODO:  在此处添加消息处理程序代码
}


void COGLGUIView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	m_OGL->m_mouseX = point.x;
	m_OGL->m_mouseY = point.y;

#ifdef USE_NOESISGUI_TEST
	m_OGL->_view->MouseMove(point.x, point.y);
#endif // USE_NOESISGUI_TEST
	CView::OnMouseMove(nFlags, point);
}

void COGLGUIView::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef USE_NOESISGUI_TEST
	m_OGL->_view->MouseButtonDown(point.x, point.y, Noesis::MouseButton_Left);
#endif
}

void COGLGUIView::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef USE_NOESISGUI_TEST
	m_OGL->_view->MouseButtonUp(point.x, point.y, Noesis::MouseButton_Left);
#endif
}