
// TestZaGDIDrugiView.h : interface of the CTestZaGDIDrugiView class
//

#pragma once
#include<vector>
#include<memory>

struct IsoscelesElement {
	POINT coord;
	COLORREF color;
	int catet;
	float rot_angle;
	int poly_side_num;
	int poly_rot_angle;
	int poly_radius;
};

class CTestZaGDIDrugiView : public CView
{
protected: // create from serialization only
	CTestZaGDIDrugiView() noexcept;
	DECLARE_DYNCREATE(CTestZaGDIDrugiView)
	bool do_grid_draw = false;
	POINT calculate_centroid(std::vector<POINT> verteces);

	const COLORREF srafura_color = RGB(0, 0, 255);

	std::vector<IsoscelesElement> isosceles_triangles = {
		{ {144, 126}, RGB(0, 200, 0), 100, 0,    5 , 0, 8 },
		{ {138, 309}, RGB(255, 0, 0), 100, 45,   7 , 0,   14 },
		{ {68, 380}, RGB(255, 192, 203), 140, -135, 4, 0,   22 },
		{ {170, 478}, RGB(255, 165, 0), 200, -90,  8, 0, 25 },
		{ {310, 412}, RGB(0, 0, 255), 190, -45,  6, 0, 20 }
	};
	void draw_isoselses_elements(CDC* pDC, std::unique_ptr<CBrush>& new_brush, CPen& inner_pen, CPen& outer_pen);
	// Attributes
public:
	CTestZaGDIDrugiDoc* GetDocument() const;

	void draw_grid(CDC* pDC, COLORREF color, POINT origin, int grid_width, int grid_height);
	void draw_background(CDC* pDC, COLORREF color, POINT origin, int bg_width, int bg_height);

	void draw_regular_polygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void draw_parallelogram(CDC* pDC, int x0, int y0, int x1, int y1, float angle);
	POINT draw_isosceles(CDC* pDC, int x0, int y0, int catet, float rot_agnle);

	POINT calculate_incircle_center(const std::vector<POINT>& vertices, double cathetus1);

	
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMSG) override;
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTestZaGDIDrugiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestZaGDIDrugiView.cpp
inline CTestZaGDIDrugiDoc* CTestZaGDIDrugiView::GetDocument() const
   { return reinterpret_cast<CTestZaGDIDrugiDoc*>(m_pDocument); }
#endif

