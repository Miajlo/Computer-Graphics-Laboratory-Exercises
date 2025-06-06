
// GLKView.cpp : implementation of the CGLKView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLK.h"
#endif

#include "GLKDoc.h"
#include "GLKView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "vec3.h"


// CGLKView

IMPLEMENT_DYNCREATE(CGLKView, CView)

BEGIN_MESSAGE_MAP(CGLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGLKView construction/destruction

CGLKView::CGLKView()
{
	// TODO: add construction code here
	
}

CGLKView::~CGLKView() {
	
}

BOOL CGLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLKView drawing

void CGLKView::OnDraw(CDC* pDC)
{
	CGLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GL_Renderer.PrepareScene(pDC);
	GL_Renderer.DrawScene(pDC);
}


// CGLKView printing

BOOL CGLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLKView diagnostics

#ifdef _DEBUG
void CGLKView::AssertValid() const
{
	CView::AssertValid();
}

void CGLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLKDoc* CGLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLKDoc)));
	return (CGLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLKView message handlers


int CGLKView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	GL_Renderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CGLKView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	GL_Renderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CGLKView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	GL_Renderer.DestroyScene(pDC);
	ReleaseDC(pDC);	
}


BOOL CGLKView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGLKView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	GL_Renderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CGLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	Camera& camera = Camera::getInstance();
	float angleDelta = 5.0f;  // Angle increment for theta and phi
	float radiusDelta = 5.0f; // Radius increment for zoom in/out
	float moveSpeed = 1.0f;

	switch (nChar) {
	case 'W':
		camera.move(camera.getForward(), moveSpeed);
		break;
	case 'S':
		camera.move(camera.getForward(), -moveSpeed);
		break;
	case 'A':
		camera.move(camera.getRight(), -moveSpeed);
		break;
	case 'D':
		camera.move(camera.getRight(), moveSpeed);
		break;
	case VK_SPACE:
		camera.move(vec3::unitY(), moveSpeed);
		break;
	case VK_CONTROL:
		camera.move(-vec3::unitY(), moveSpeed);
		break;
	case VK_UP:
		camera.rotate(0.0f, angleDelta); // Look up
		break;
	case VK_DOWN:
		camera.rotate(0.0f, -angleDelta); // Look down
		break;
	case VK_RIGHT:
		camera.rotate(-angleDelta, 0.0f); // Look left
		break;
	case VK_LEFT:
		camera.rotate(angleDelta, 0.0f); // Look right
		break;
	}

	Invalidate(); // Redraw the view
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
