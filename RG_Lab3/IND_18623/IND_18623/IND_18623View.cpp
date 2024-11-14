
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

	translate(pDC, -220, -180, right_mult);
	rotate(pDC,109, right_mult);
	
	mirror(pDC, true,false, right_mult);
	translate(pDC, 25, 7, right_mult);


	DImage part1;
	
	part1.Load(_T("res\\Mon_0_0.bmp"));
	CRect rect(0 ,0, 256,256);
	auto btm = part1.GetBitmap();
	COLORREF color = RGB(0, 255, 0);
	DrawTransparentImage(part1, pDC, rect, rect, color);
		
	//part1_trans.Draw(pDC, rect, CRect(30,30, 286, 286));
	pDC->SetWorldTransform(&old_transform);
	
	translate(pDC, -32, -87, right_mult);
	rotate(pDC, 17, right_mult);
	mirror(pDC, false, true, right_mult);
	translate(pDC, 318, 8, right_mult);


	DImage part2;

	part2.Load(_T("res\\Mon_0_1.bmp"));
	DrawTransparentImage(part2, pDC, rect, rect, color);
	pDC->SetWorldTransform(&old_transform);

	
	translate(pDC, -90, -230, right_mult);
	rotate(pDC, 115, right_mult);
	mirror(pDC, false, true, right_mult);
	translate(pDC, 465, 10, right_mult);


	DImage part3;

	part3.Load(_T("res\\Mon_0_2.bmp"));
	DrawTransparentImage(part3, pDC, rect, rect, color);
	pDC->SetWorldTransform(&old_transform);

	translate(pDC, -223, -175, right_mult);
	rotate(pDC, 203, right_mult);
	mirror(pDC, true, false, right_mult);
	translate(pDC, 22, 307, right_mult);


	DImage part4;

	part4.Load(_T("res\\Mon_1_0.bmp"));
	DrawTransparentImage(part4, pDC, rect, rect, color);
	pDC->SetWorldTransform(&old_transform);

		
	translate(pDC, -150, -240, right_mult);
	rotate(pDC, -32, right_mult);
	mirror(pDC, false, true, right_mult);
	translate(pDC, 169, 307, right_mult);


	DImage part5;

	part5.Load(_T("res\\Mon_1_1.bmp"));
	DrawTransparentImage(part5, pDC, rect, rect, color);
	pDC->SetWorldTransform(&old_transform);

	translate(pDC, -150, -240, right_mult);
	rotate(pDC, 145, right_mult);
	mirror(pDC, false, true, right_mult);
	translate(pDC, 475, 160, right_mult);


	DImage part6;

	part6.Load(_T("res\\Mon_1_2.bmp"));
	DrawTransparentImage(part6, pDC, rect, rect, color);
	pDC->SetWorldTransform(&old_transform);

	pDC->SetGraphicsMode(old_mode);


}


void CIND18623View::DrawTransparentImage(DImage& img, CDC* pDC, CRect rcImg, CRect rcDC, COLORREF clrTransparent) {
	// Create a memory device context (DC) compatible with the destination DC.
	CDC* pMemDC = new CDC();
	if (!pMemDC->CreateCompatibleDC(pDC)) {
		delete pMemDC;
		return; // Return if creation failed
	}

	// Select the bitmap into the memory DC.
	pMemDC->SelectObject(img.GetBitmap()); // Use the bitmap from DImage

	// Set the stretching mode for the destination DC to HALFTONE.
	pDC->SetStretchBltMode(HALFTONE); // Smooth the bitmap during stretching

	// Perform the TransparentBlt operation to copy the image to the destination.
	// TransparentBlt takes in the destination rect, source rect, and the transparent color.
	pDC->TransparentBlt(rcDC.left, rcDC.top,
		rcDC.Width(), rcDC.Height(),
		pMemDC, rcImg.left, rcImg.top,
		rcImg.Width(), rcImg.Height(),
		clrTransparent);

	delete pMemDC;
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
