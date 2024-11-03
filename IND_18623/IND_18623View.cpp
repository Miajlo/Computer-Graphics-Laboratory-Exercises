
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
	ON_WM_KEYDOWN()
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

void CIND18623View::draw_flower_pot(CDC *pDC) {
	CBrush nova(RGB(222, 148, 0));
	auto old_brush = pDC->SelectObject(&nova);

	pDC->Polygon(trapezoid.data(), trapezoid.size());
	pDC->Rectangle(rectangle[0].x, rectangle[0].y, rectangle[1].x, rectangle[1].y);

	pDC->SelectObject(old_brush);
}

void CIND18623View::draw_grid(CDC* pDC, int &grid_width, int &grid_height, int &grid_unit_size) {
	CPen* OldPen;
	CPen gridPen(BS_SOLID, 1, RGB(255, 255, 255));
	OldPen = pDC->SelectObject(&gridPen);
	int x_offset = 25;
	int y_offset = 25;
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

void CIND18623View::draw_background(CDC* pDC, int& bg_width, int& bg_height) {
	CBrush new_brush(RGB(135, 206, 235));
	
	auto old_brush = pDC->SelectObject(&new_brush);
	
	pDC->Rectangle(0, 0, bg_width, bg_height);

	pDC->SelectObject(old_brush);
	old_brush = nullptr;
}

void CIND18623View::draw_figure(CDC* pDC) {
	CString green_path = _T("cactus_part.emf");
	HENHMETAFILE green_part = GetEnhMetaFile(green_path);
	CString yellow_path = _T("cactus_part_light.emf");
	HENHMETAFILE yellow_part = GetEnhMetaFile(yellow_path);



	draw_cactus_elements(pDC, green_part, yellow_part);

	draw_elipses(pDC);

}

void CIND18623View::draw_elipses(CDC* pDC) {
	CBrush nova_ceta(RGB(0, 204, 0));
	auto old_brush = pDC->SelectObject(&nova_ceta);
	
	int old_mode = pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM old_transform;
	pDC->GetWorldTransform(&old_transform);
	bool right_mult = true;

	for (const auto& elipse : elipse_coords) {
		translate(pDC, elipse.x - all_rot_point.x, elipse.y - all_rot_point.y, right_mult);
		rotate(pDC, all_obj_rot_angle, right_mult);
		translate(pDC, all_rot_point.x, all_rot_point.y, right_mult);

		pDC->Ellipse(elipse.x, elipse.y, elipse.x + elipse_size, elipse.y + elipse_size);

		pDC->SetWorldTransform(&old_transform);
	}

	pDC->SelectObject(old_brush);
	pDC->SetGraphicsMode(old_mode);
}

void CIND18623View::draw_cactus_elements(CDC* pDC, HENHMETAFILE& green_part, HENHMETAFILE& yellow_part) {
	int old_mode = pDC->SetGraphicsMode(GM_ADVANCED);
	float angle = 45;
	XFORM old_transform;
	pDC->GetWorldTransform(&old_transform);
	bool right_mult = true;




	for (const auto& elem : cactus_elements) {
		scale(pDC, elem.sx, elem.sy, right_mult);
		rotate(pDC, elem.angle, right_mult);
		translate(pDC, elem.position.x - all_rot_point.x,
			elem.position.y - all_rot_point.y, right_mult);
		rotate(pDC, all_obj_rot_angle, right_mult);
		translate(pDC, all_rot_point.x, all_rot_point.y, right_mult);

		pDC->PlayMetaFile(elem.type == ObjectType::GREEN_PART ?
			green_part : yellow_part, CRect(-109, 0, 109, -209));


		pDC->SetWorldTransform(&old_transform);
	}
	pDC->SetGraphicsMode(old_mode);
}

void CIND18623View::translate(CDC* pDC, float dX, float dY, bool right_multiply) {

	XFORM translate = transforms::get_translation_matrix(dX, dY);

	pDC->ModifyWorldTransform(&translate, right_multiply ? MWT_RIGHTMULTIPLY 
														 : MWT_LEFTMULTIPLY);

}

void CIND18623View::scale(CDC* pDC, float sX, float sY, bool right_multiply) {
	XFORM scale = transforms::get_scaling_matrix(sX, sY);

	pDC->ModifyWorldTransform(&scale, right_multiply ? MWT_RIGHTMULTIPLY
													 : MWT_LEFTMULTIPLY);
}

void CIND18623View::rotate(CDC* pDC, float angle, bool right_multiply) {
	XFORM rotation = transforms::get_rotational_matrix(angle);

	pDC->ModifyWorldTransform(&rotation, right_multiply ? MWT_RIGHTMULTIPLY
														: MWT_LEFTMULTIPLY);
}

void CIND18623View::OnDraw(CDC* pDC)
{
	CIND18623Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	int g_width = 500, g_height = 500, g_unit_size = 20;
	int bg_width = g_width, bg_height = g_height;


	draw_background(pDC, bg_width, bg_height);

	CBrush nova_ceta(RGB(0, 204, 0));
	auto old_brush = pDC->SelectObject(&nova_ceta);
	pDC->Ellipse(239, 416, 261, 436);
	pDC->SelectObject(old_brush);

	draw_figure(pDC);

	draw_elipses(pDC);

	draw_flower_pot(pDC);

	

	

	if (do_grid_draw)
		draw_grid(pDC, g_width, g_height, g_unit_size);
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

void CIND18623View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	bool redraw_requierd = false;

	switch (nChar) {
		case 0x47: //G
			do_grid_draw = !do_grid_draw;
			redraw_requierd = true;
			break;
		case 0x51: //q
			cactus_elements[0].angle += 5;
			redraw_requierd = true;
			break;

		case 0x45: //e
			cactus_elements[0].angle -= 5;
			redraw_requierd = true;
			break;

		case 0x41: //a
			all_obj_rot_angle += 5;
			redraw_requierd = true;
			break;

		case 0x44: //d
			all_obj_rot_angle -= 5;
			redraw_requierd = true;
			break;
	}
	if(redraw_requierd)
		Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
