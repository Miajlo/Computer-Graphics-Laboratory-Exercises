
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
#include<cmath>
#include <corecrt_math_defines.h>





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

	auto rad_angle = angle * M_PI / 180;

	auto cosin = std::cos(rad_angle);
	auto sinus = std::sin(rad_angle);

	trans_matrix.eM11 = cosin;
	trans_matrix.eM12 = sinus;
	trans_matrix.eM21 = -sinus;
	trans_matrix.eM22 = cosin;
	trans_matrix.eDx = 0;
	trans_matrix.eDy = 0;

	pDC->ModifyWorldTransform(&trans_matrix, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18623View::scale(CDC* pDC, float sx, float sy, bool right_mult) {
	
	trans_matrix.eM11 = sx;
	trans_matrix.eM12 = 0;
	trans_matrix.eM21 = 0;
	trans_matrix.eM22 = sy;
	trans_matrix.eDx = 0;
	trans_matrix.eDy = 0;

	pDC->ModifyWorldTransform(&trans_matrix, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18623View::translate(CDC* pDC, float dx, float dy, bool right_mult) {
	trans_matrix.eM11 = 1;
	trans_matrix.eM12 = 0;
	trans_matrix.eM21 = 0;
	trans_matrix.eM22 = 1;
	trans_matrix.eDx = dx;
	trans_matrix.eDy = dy;

	pDC->ModifyWorldTransform(&trans_matrix, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18623View::mirror(CDC* pDC, bool mx, bool my, bool right_mult) {
	trans_matrix.eM11 = mx ? 1 : -1;
	trans_matrix.eM12 = 0;
	trans_matrix.eM21 = 0;
	trans_matrix.eM22 = my ? 1 : -1;
	trans_matrix.eDx = 0;
	trans_matrix.eDy = 0;

	pDC->ModifyWorldTransform(&trans_matrix, right_mult ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CIND18623View::OnDraw(CDC* pDC)
{
	CIND18623Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	auto old_mode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old_transform;
	pDC->GetWorldTransform(&old_transform);

	translate(pDC, -158, -158, right_mult);
	rotate(pDC, 200, right_mult);
	//mirror(pDC, false, false, true);
	translate(pDC, 158, 158, right_mult);


	DImage part1;
	
	part1.Load(_T("res\\Mon_0_0.bmp"));
	CRect rect(0,0, 256,256);
	auto btm = part1.GetBitmap();
	COLORREF color = RGB(0, 255, 0);
	bitmap_make_transparent(part1, color, pDC);
	/*DImage part1_trans(*btn_trans);
	
	part1_trans.Draw(pDC, rect, CRect(30,30, 286, 286));*/
	pDC->SetWorldTransform(&old_transform);
	pDC->SetGraphicsMode(old_mode);
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

CBitmap* CIND18623View::bitmap_make_transparent(DImage& img, COLORREF transparentColor, CDC* pDC) {
	if (!img.isValid()) {
		return nullptr;
	}

	CBitmap bmp_mask;
	BITMAP bm;
	img.GetBitmap()->GetBitmap(&bm);

	bmp_mask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* old_src_bmp = srcDC->SelectObject(img.GetBitmap());
	CBitmap* old_dst_bmp = dstDC->SelectObject(img.GetBitmap());

	COLORREF clr_top_left = srcDC->GetPixel(0, 0);
	COLORREF clr_save_bk = srcDC->SetBkColor(clr_top_left);
	dstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCCOPY);

	COLORREF clr_save_dst_text = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0)); //za and operaciju
	srcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);

	dstDC->SetTextColor(clr_save_dst_text);
	srcDC->SetBkColor(clr_save_bk);
	srcDC->SelectObject(old_src_bmp);
	dstDC->SelectObject(old_dst_bmp);
	delete srcDC;
	delete dstDC;

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap* bmp_old = memDC->SelectObject(&bmp_mask);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCAND);

	memDC->SelectObject(img.GetBitmap());
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCPAINT);
	memDC->SelectObject(bmp_old);
	delete memDC;

	return nullptr;
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
