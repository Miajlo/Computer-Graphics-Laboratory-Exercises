
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
	ON_WM_KEYDOWN()
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

void CIND18623View::draw_img(CDC* pDC) {
	COLORREF color(RGB(0, 255, 0));
	CRect rect(0, 0, 256, 256);
	CRect img_rect(0, 0, 510, 510);
	CDC memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return;

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, img_rect.Width(), img_rect.Height());
	memDC.SelectObject(&memBitmap);

	// Fill the memory DC with white background
	memDC.FillSolidRect(0, 0, img_rect.Width(), img_rect.Height(), RGB(255, 255, 255));

	auto old_mode = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM old_transform;
	memDC.GetWorldTransform(&old_transform);

	if (do_grid_draw)
		draw_grid(&memDC);


	for (int i = 0; i < imageParts.size(); ++i) {
		for (int j = 0; j < imageParts[i].size(); ++j) {
			auto part = imageParts[i][j];

			scale(&memDC, sx, sy, right_mult);
			translate(&memDC, part.translate1.x, part.translate1.y, right_mult);
			rotate(&memDC, part.rotationAngle, right_mult);
			mirror(&memDC, part.mirrorX, part.mirrorY, right_mult);
			translate(&memDC, part.translate2.x, part.translate2.y, right_mult);

			if (center_rot_angle) {
				translate(&memDC, -rot_center.x, -rot_center.y, right_mult);
				rotate(&memDC, center_rot_angle, right_mult);
				translate(&memDC, rot_center.x, rot_center.y, right_mult);
			}

			std::string path = path_base + std::to_string(i) +
				'_' + std::to_string(j) + file_ext;

			// Load and draw the image
			DImage image;
			image.Load(CString(path.c_str()));
			DrawTransparentImage(image, &memDC, rect, rect, color, part.blue_filter);

			// Restore the original transformation
			memDC.SetWorldTransform(&old_transform);

		}
	}

	pDC->BitBlt(0, 0, img_rect.Width(), img_rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SetGraphicsMode(old_mode);
	memDC.DeleteDC();
}

void CIND18623View::draw_grid(CDC* pDC) {
	CPen* OldPen;
	CPen gridPen(BS_SOLID, 1, GRID_COLOR);
	OldPen = pDC->SelectObject(&gridPen);

	for (int i = 0; i <= grid_width / grid_unit_size; i++) {
		pDC->MoveTo(grid_unit_size * i, 0);
		pDC->LineTo(grid_unit_size * i, grid_height);
	}
	for (int i = 0; i <= grid_height / grid_unit_size; i++) {
		pDC->MoveTo(0, grid_unit_size * i);
		pDC->LineTo(grid_width, grid_unit_size * i);
	}

	pDC->SelectObject(OldPen);
}

void CIND18623View::OnDraw(CDC* pDC)
{
	CIND18623Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	draw_img(pDC);

}


void CIND18623View::DrawTransparentImage(DImage& img, CDC* pDC, CRect &rcImg, CRect &rcDC, COLORREF &clrTransparent, const bool &blue_filter) {
	// Retrieve the bitmap from the image
	CBitmap* pBitmap = img.GetBitmap();
	if (!pBitmap) {
		return; // Return if the bitmap is invalid
	}

	
	// Get bitmap information
	BITMAP bmp;
	pBitmap->GetBitmap(&bmp);

	// Calculate the size of the bitmap data
	int dataSize = bmp.bmWidthBytes * bmp.bmHeight;

	// Allocate memory to hold the bitmap data
	std::vector<BYTE> bitmapData(dataSize);

	// Get the bitmap bits
	if (pBitmap->GetBitmapBits(dataSize, bitmapData.data()) == 0) {
		return; // Failed to retrieve bitmap bits
	}

	// Modify the bitmap bits
	for (int y = 0; y < bmp.bmHeight; ++y) {
		for (int x = 0; x < bmp.bmWidth; ++x) {
			// Calculate the pixel's byte offset
			int pixelOffset = y * bmp.bmWidthBytes + x * (bmp.bmBitsPixel / 8);

			// Extract pixel color (assuming 24-bit RGB format)
			BYTE blue = bitmapData[pixelOffset];
			BYTE green = bitmapData[pixelOffset + 1];
			BYTE red = bitmapData[pixelOffset + 2];

			COLORREF color = RGB(red, green, blue);

			if (color != clrTransparent) {
				int newRed = 0, newGreen = 0, newBlue = 0;

				if (blue_filter) {
					newBlue = std::min<int>(64 + blue, 255);
				}
				else {
					auto gray = std::min<int>(64 + (red + green + blue) / 3,255);
					newRed = newGreen = newBlue = gray;
				}

				// Set the new pixel color
				bitmapData[pixelOffset] = newBlue;
				bitmapData[pixelOffset + 1] = newGreen;
				bitmapData[pixelOffset + 2] = newRed;
			}
		}
	}

	// Set the modified bits back to the bitmap
	if (pBitmap->SetBitmapBits(dataSize, bitmapData.data()) == 0) {
		return; // Failed to set bitmap bits
	}

	// Create a memory DC for the bitmap
	CDC memDC;
	if (!memDC.CreateCompatibleDC(pDC)) {
		return; // Failed to create compatible DC
	}

	// Select the bitmap into the memory DC
	CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

	
	pDC->SetStretchBltMode(HALFTONE); // Smooth the bitmap during stretching

	
	pDC->TransparentBlt(rcDC.left, rcDC.top,
		rcDC.Width(), rcDC.Height(),
		&memDC, rcImg.left, rcImg.top,
		rcImg.Width(), rcImg.Height(),
		clrTransparent);

	// Restore the old bitmap in the memory DC
	memDC.SelectObject(pOldBitmap);
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


void CIND18623View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case 'R':
		UpdateWindow();
		break;
	case 'G':
		do_grid_draw = !do_grid_draw;
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
