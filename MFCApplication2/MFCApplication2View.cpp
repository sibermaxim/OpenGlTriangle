// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MFCApplication2View.cpp : implementation of the CMFCApplication2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_BUTTON2, &CMFCApplication2View::OnButton2)
END_MESSAGE_MAP()

// CMFCApplication2View construction/destruction

CMFCApplication2View::CMFCApplication2View()
{
	bln_MouseLclick = false;
	m_Graphics = nullptr;

	p_popup = new CMenu;
	p_popup->LoadMenuW(IDR_MENU1);
}

CMFCApplication2View::~CMFCApplication2View()
{
	if(p_popup)
	{
		p_popup->DestroyMenu();
		delete p_popup;
	}
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_OVERLAPPED |
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS;
	return CView::PreCreateWindow(cs);
}

// CMFCApplication2View drawing

void CMFCApplication2View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}


// CMFCApplication2View printing


void CMFCApplication2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCApplication2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication2View diagnostics

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication2View message handlers


void CMFCApplication2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_ogl = new COgl(this);
	m_ogl->Initializze();
	m_ogl->InitializeOpenGL(width, height, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	m_Graphics->Initialize(m_ogl, this->m_hWnd);
}


void CMFCApplication2View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CPoint point;
	point.x = cx;
	point.y = cy;

	ClientToScreen(&point);
	width = cx;
	height = cy;
}


void CMFCApplication2View::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
	if(m_Graphics)
		m_Graphics->Frame();
}


void CMFCApplication2View::OnClose()
{

	CView::OnClose();
}


void CMFCApplication2View::OnDestroy()
{
	CView::OnDestroy();

	m_Graphics->Shutdown();
	delete m_Graphics;

	delete m_ogl;
	// TODO: Add your message handler code here
}


void CMFCApplication2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (!bln_MouseLclick)
		return;

	if(_thatMouse == 1)
	{
		glm::vec3 secVec= glm::vec3(point.x, point.y, 0), rotAxis;

		//secVec = glm::normalize(secVec);
		//_prevPosition = glm::normalize(_prevPosition);

		rotAxis = glm::cross(_prevPosition, secVec);
		double ag = glm::dot(_prevPosition, secVec);
		rotAngle += ag;

	}
	else if(_thatMouse == 0)
	{
		m_Graphics->MouseMove(point);
	}
	CView::OnMouseMove(nFlags, point);
}


void CMFCApplication2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
//	ClientToScreen(&point);
	CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetApp()->m_pMainWnd);
	_thatMouse = pFrame->ThatMouse;

	_mousePressPos.x = point.x;
	_mousePressPos.y = point.y;
	bln_MouseLclick = true;

	if(_thatMouse == 0)
	{
		bln_MouseLclick = true;
		m_Graphics->MouseLDown(point);
	}
	else if(_thatMouse == 1)
	{
		_prevPosition = glm::vec3(point.x, point.y, 0);
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMFCApplication2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
//	ClientToScreen(&point);
	m_Graphics->MouseLUp(point);
	bln_MouseLclick = false;

	CView::OnLButtonUp(nFlags, point);
}


BOOL CMFCApplication2View::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::OnWndMsg(message, wParam, lParam, pResult);
}


void CMFCApplication2View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CMFCApplication2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (isupper(nChar))
		nChar = tolower(nChar);

	m_Graphics->Update(nChar, nRepCnt, nFlags);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFCApplication2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ClientToScreen(&point);
	CMenu *psub = p_popup->GetSubMenu(0);

	if (psub) // Если такое подменю существует
	{
		// Отобразить его в том месте, где щелкнули правой кнопкой мыши
		psub->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	CView::OnRButtonDown(nFlags, point);
}


void CMFCApplication2View::OnButton2()
{
	// TODO: Add your command handler code here
}
