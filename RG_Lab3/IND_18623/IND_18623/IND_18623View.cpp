
// IND_18623View.cpp : implementation of the CIND18623View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18623.h"
#endif

#include "IND_18623Doc.h"
#include "IND_18623View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND18623View

IMPLEMENT_DYNCREATE(CIND18623View, CView)

BEGIN_MESSAGE_MAP(CIND18623View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIND18623View construction/destruction

CIND18623View::CIND18623View() noexcept
{
	// TODO: add construction code here

}

CIND18623View::~CIND18623View()
{
}

BOOL CIND18623View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18623View drawing

void CIND18623View::rotate(CDC* pDC, float angle, bool right_mult) {
}

void CIND18623View::scale(CDC* pDC, float sx, float sy, bool right_mult)
{
}

void CIND18623View::translate(CDC* pDC, float dx, float dy, bool right_mult)
{
}

void CIND18623View::mirror(CDC* pDC, bool mx, bool my, bool right_mult)
{
}

void CIND18623View::OnDraw(CDC* /*pDC*/)
{
	CIND18623Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CIND18623View printing

BOOL CIND18623View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18623View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18623View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18623View diagnostics

#ifdef _DEBUG
void CIND18623View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18623View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18623Doc* CIND18623View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18623Doc)));
	return (CIND18623Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18623View message handlers
