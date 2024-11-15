
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

void CIND18623View::load_puzzle() {

	for (auto i = 0; i < rows; ++i) {
		std::vector<DImage> row_to_add;
		for (auto j = 0; j < cols; ++j) {
			DImage tmp;
			std::ostringstream oss;
			oss << bitmaps_base << i << '_' << j << ".bmp";
			tmp.Load(CString(oss.str().c_str()));
			row_to_add.push_back(tmp);
		}
		puzzle_parts.push_back(row_to_add);
	}

}

void CIND18623View::draw_puzzle(CDC* pDC) {

	for (auto i = 0; i < rows; ++i) {
		for (auto j = 0; j < cols; ++j) {

		}
	}

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
	COLORREF color(RGB(0, 255, 0));
	CRect rect(0, 0, 256, 256);
	for (const auto& part : imageParts) {
		translate(pDC, part.translate1.x, part.translate1.y, right_mult);
		rotate(pDC, part.rotationAngle, right_mult);
		mirror(pDC, part.mirrorX, part.mirrorY, right_mult);
		translate(pDC, part.translate2.x, part.translate2.y, right_mult);

		if (center_rot_angle) {
			translate(pDC, -rot_center.x, -rot_center.y, right_mult);
			rotate(pDC, center_rot_angle, right_mult);
			translate(pDC, rot_center.x, rot_center.y, right_mult);
		}

		// Load and draw the image
		DImage image;
		image.Load(part.imagePath);
		DrawTransparentImage(image, pDC, rect, rect, color, part.blue_filter);

		// Restore the original transformation
		pDC->SetWorldTransform(&old_transform);
	}

	pDC->SetGraphicsMode(old_mode);


}


void CIND18623View::DrawTransparentImage(DImage& img, CDC* pDC, CRect &rcImg, CRect &rcDC, COLORREF &clrTransparent, const bool &blue_filter) {
	// Create a memory device context (DC) compatible with the destination DC.
	CDC* pMemDC = new CDC();
	if (!pMemDC->CreateCompatibleDC(pDC)) {
		delete pMemDC;
		return; // Return if creation failed
	}

	pMemDC->SelectObject(img.GetBitmap()); // Use the bitmap from DImage

	
	for (int y = 0; y < rcImg.Height(); y++) {
		for (int x = 0; x < rcImg.Width(); x++) {
			COLORREF color = pMemDC->GetPixel(x, y);
			if (color != clrTransparent) {
				auto pixel = pMemDC->GetPixel(x,y);
				auto R = GetRValue(pixel);
				auto G = GetGValue(pixel);
				auto B = GetBValue(pixel);

				COLORREF new_color;

				if (blue_filter) {
					auto blue = 64 + B;
					blue = blue > 255 ? 255 : blue;
					new_color = RGB(0, 0, blue);
				}
				else {
					auto gr = 64 + (R + G + B) / 3;
					gr = gr > 255 ? 255 : gr;
					new_color = RGB(gr, gr, gr);
				}


				pMemDC->SetPixel(x, y, new_color);

			}
		}
	}

	// Select the bitmap into the memory DC.
	

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
