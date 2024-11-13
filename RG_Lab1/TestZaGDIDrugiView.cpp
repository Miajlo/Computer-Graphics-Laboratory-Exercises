
// TestZaGDIDrugiView.cpp : implementation of the CTestZaGDIDrugiView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestZaGDIDrugi.h"
#endif

#include "TestZaGDIDrugiDoc.h"
#include "TestZaGDIDrugiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
#include <corecrt_math_defines.h>
#include <memory>
#include <iostream>


// CTestZaGDIDrugiView

IMPLEMENT_DYNCREATE(CTestZaGDIDrugiView, CView)

BEGIN_MESSAGE_MAP(CTestZaGDIDrugiView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestZaGDIDrugiView construction/destruction

CTestZaGDIDrugiView::CTestZaGDIDrugiView() noexcept
{
	// TODO: add construction code here

}

POINT CTestZaGDIDrugiView::calculate_centroid(std::vector<POINT> verteces)
{
	int x = 0, y = 0;
	for (const auto& vert : verteces) {
		x += vert.x;
		y += vert.y;
	}
	x /= verteces.size();
	y /= verteces.size();
	return { x, y };
}

CTestZaGDIDrugiView::~CTestZaGDIDrugiView()
{
}

BOOL CTestZaGDIDrugiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

BOOL CTestZaGDIDrugiView::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN) { // Check for key down message
		switch (pMsg->wParam)  { // Check the key code
		case 'G': // Check if the 'G' key is pressed
			// Handle 'G' key press here
			do_grid_draw = !do_grid_draw;
			Invalidate();
			return TRUE; // Return TRUE to indicate it was handled
		}
	}
	return FALSE;
}

// CTestZaGDIDrugiView drawing

void CTestZaGDIDrugiView::OnDraw(CDC* pDC) {
	CTestZaGDIDrugiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int grid_width = GetSystemMetrics(SM_CXSCREEN), grid_height = GetSystemMetrics(SM_CYSCREEN);

	int x0 = 98, y0 = 115, r0 = 40, n0 = 3, angle0 = 180, catet0 = 70;
	int out_poly_line_size = 6, in_poly_line_size = 3;
	int parall_angle = 45, suqare_rad = 72, square_rot_angle = 90;
	int bg_width = grid_width, bg_height = grid_height;
	POINT origin({ 4, 4 }), parall0({ 144, 25 }), parall1({ 216, 235 });
	POINT square_cord({ 142, 235 });
	COLORREF outline_color(RGB(82, 250, 210)), yellow(RGB(255, 255, 0)), purple(RGB(255, 0, 255));
	COLORREF bg_color(RGB(211, 211, 211)), grid_color(RGB(255, 255, 255));

	std::unique_ptr<CBrush> new_brush = std::make_unique<CBrush>(yellow);

	CPen outer_pen(BS_SOLID, out_poly_line_size, outline_color);
	CPen inner_pen(BS_SOLID, in_poly_line_size, outline_color);

	if (!pDoc)
		return;

	//draw background
	draw_background(pDC, bg_color, origin, bg_width, bg_height);

	auto old_pen = pDC->SelectObject(&outer_pen);
	auto old_brush = pDC->SelectObject(new_brush.get());

	//draw all isosceles elments, tirangle and inside polygon
	draw_isoselses_elements(pDC, new_brush, inner_pen, outer_pen);

	new_brush.reset(new CBrush(yellow));

	pDC->SelectObject(new_brush.get());
	pDC->SelectObject(&outer_pen);

	// draw parallelogram
	draw_parallelogram(pDC, parall0.x, parall0.y,parall1.x, parall1.y, parall_angle); 

	new_brush.reset(new CBrush(purple));
	pDC->SelectObject(new_brush.get());

	// draw the square
	draw_regular_polygon(pDC, square_cord.x, square_cord.y, suqare_rad, 4, square_rot_angle); 

	old_brush = pDC->SelectObject(old_brush);
	old_pen = pDC->SelectObject(old_pen);

	if (do_grid_draw)
		draw_grid(pDC, grid_color, origin, grid_width, grid_height);
}

void CTestZaGDIDrugiView::draw_isoselses_elements(CDC* pDC, std::unique_ptr<CBrush>& new_brush, CPen &inner_pen, CPen &outer_pen) {
	for (const auto& element : isosceles_triangles) {
		new_brush.reset(element.color != srafura_color ? new CBrush(element.color)
			: new CBrush(HS_BDIAGONAL, element.color));
		pDC->SelectObject(&outer_pen);
		pDC->SelectObject(new_brush.get());

		auto tc_coord = draw_isosceles(pDC, element.coord.x, element.coord.y,
			element.catet, element.rot_angle);

		pDC->SelectObject(&inner_pen);

		draw_regular_polygon(pDC, tc_coord.x, tc_coord.y, element.poly_radius,
			element.poly_side_num, element.poly_rot_angle);
	}
}

void CTestZaGDIDrugiView::draw_grid(CDC* pDC, COLORREF color, POINT origin, int grid_width, int grid_height) {
	CBrush grid_brush(HS_CROSS, color);

	auto old_pen = pDC->SelectObject(&grid_brush);

	pDC->SetBkMode(TRANSPARENT);

	pDC->SetBrushOrg(origin);

	pDC->Rectangle(0, 0, grid_width, grid_height);

	pDC->SelectObject(old_pen);

	pDC->SetBrushOrg(0, 0);

	pDC->SetBkMode(OPAQUE);
}

void CTestZaGDIDrugiView::draw_background(CDC* pDC, COLORREF color, POINT origin, int bg_width, int bg_height) {
	CBrush bg_brush(color);

	auto old_pen = pDC->SelectObject(&bg_brush);

	pDC->SetBrushOrg(origin);

	pDC->Rectangle(0, 0, bg_width, bg_height);

	pDC->SelectObject(old_pen);

	pDC->SetBrushOrg(0, 0);
}

void CTestZaGDIDrugiView::draw_regular_polygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	std::vector<POINT> points;

	double curr_angle = 2 * M_PI / n;
	double rot_angle = rotAngle * M_PI / 180;
	for (auto i = 0; i < n; ++i)
			points.push_back({ cx + static_cast<int>((r * cos(curr_angle * i + rot_angle))),
						cy + static_cast<int>(r * sin(curr_angle * i + rot_angle))});
	


	pDC->Polygon(points.data(), points.size());
}

void CTestZaGDIDrugiView::draw_parallelogram(CDC* pDC, int x0, int y0, int x1, int y1, float angle) {
	std::vector<POINT> vertices;

	int y_dist = y1 - y0;

	int x_dist = x1 - x0;
	float p_angle = /*M_PI / 2 -*/ (angle * M_PI / 180);

	int cut = x_dist / tan(p_angle);
	//int y_cut = x_dist / tan(p_angle);

	vertices.push_back({ x0, y0 });                // Top-left
	vertices.push_back({ x0 , y1 - cut });          // Top-right (offset horizontally)
	vertices.push_back({ x1 , y1 });          // Bottom-right (offset horizontally)
	vertices.push_back({ x1, y0 + cut});                // Bottom-left


	pDC->Polygon(vertices.data(), vertices.size());
}

POINT CTestZaGDIDrugiView::draw_isosceles(CDC* pDC, int x0, int y0, int catet, float rot_angle) {

	std::vector<POINT> verteces = { {x0, y0}, {x0 - catet, y0}, {x0, y0 - catet} };

	rot_angle *= M_PI / 180;

	int r = (2 * catet + sqrt(2 * catet * catet)) / 2;
		
	const auto right_point = verteces[0];

	for (auto& vert : verteces) {
		
		int original_x = vert.x - right_point.x;
		int original_y = vert.y - right_point.y;

		// Apply the rotation matrix
		vert.x = static_cast<int>(cos(rot_angle) * original_x + sin(rot_angle) * original_y);
		vert.y = static_cast<int>(-sin(rot_angle) * original_x + cos(rot_angle) * original_y);
	
		vert.x += right_point.x;
		vert.y += right_point.y;
	}

	pDC->Polygon(verteces.data(), verteces.size());
	
	const auto centroid = calculate_incircle_center(verteces, catet);

	return centroid;
}


POINT CTestZaGDIDrugiView::calculate_incircle_center(const std::vector<POINT>&vertices, double catet) {
	POINT rightAngle = vertices[0];

	POINT A = vertices[0];  // Right-angle vertex
	POINT B = vertices[1];  // One leg vertex
	POINT C = vertices[2];  // Other leg vertex

	
	double c = sqrt(pow(B.x - C.x, 2) + pow(B.y - C.y, 2)); // length opposite A (hypotenuse)


	// Calculate the incenter coordinates using the incenter formula
	POINT incenter;
	incenter.x = static_cast<int>((c * A.x + catet * (B.x + C.x)) / (c + 2* catet));
	incenter.y = static_cast<int>((c * A.y + catet * (B.y + C.y)) / (c + 2 * catet));

	return incenter;
}


// CTestZaGDIDrugiView printing

BOOL CTestZaGDIDrugiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestZaGDIDrugiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestZaGDIDrugiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTestZaGDIDrugiView diagnostics

#ifdef _DEBUG
void CTestZaGDIDrugiView::AssertValid() const
{
	CView::AssertValid();
}

void CTestZaGDIDrugiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestZaGDIDrugiDoc* CTestZaGDIDrugiView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestZaGDIDrugiDoc)));
	return (CTestZaGDIDrugiDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestZaGDIDrugiView message handlers
