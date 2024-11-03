
// IND_18623View.h : interface of the CIND18623View class
//

#pragma once
#include <string>
#include"transforms.h"
#include<vector>

enum class ObjectType {
	GREEN_PART,
	YELLOW_PART,
};

class cactus_element {
public:
	CPoint position;
	float sx; //scale
	float sy;
	float angle;
	ObjectType type;
};

class CIND18623View : public CView
{
protected: // create from serialization only
	CIND18623View() noexcept;
	DECLARE_DYNCREATE(CIND18623View)
	
// Attributes
public:
	CIND18623Doc* GetDocument() const;
	void draw_flower_pot(CDC* pDC);
// Operations
public:
	void draw_grid(CDC *pDC, int &grid_width, int &grid_height, int &grid_unit_size);
	void draw_background(CDC* pDC, int& bg_width, int& bg_height);

	void draw_figure(CDC* pDC);
	void draw_elipses(CDC* pDC);
	void draw_cactus_elements(CDC* pdc, HENHMETAFILE& green_part, HENHMETAFILE& yellow_part);

	void translate(CDC* pDC, float dX, float dY, bool right_multiply);
	void scale(CDC* pDC, float sX, float sY, bool right_multiply);
	void rotate(CDC* pDC, float angle, bool right_multiply);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	bool do_grid_draw = false;


	std::vector<CPoint> trapezoid = { {210, 500}, {291, 500}, {300, 450}, {200, 450} };
	std::vector<CPoint> rectangle = { {188,450}, {310, 430} };
	
	const int metaf_y_offset = 209;
	const int metaf_x_offset = 109;

	const CPoint all_rot_point = { 250, 426 };
	const COLORREF BG_COLOR = RGB(135, 206, 235);
	const COLORREF GRID_COLOR = RGB(255, 255, 255);
	const COLORREF ELIPSE_COLOR = RGB(0, 204, 0);
	const COLORREF FLOWER_POT_COLOR = RGB(222, 148, 0);

	std::vector<cactus_element> cactus_elements = {
		{ CPoint(300, 305), 0.2f, 0.35f, 0, ObjectType::YELLOW_PART },
		{ CPoint(250, 425), 0.3f, 0.35f, 0, ObjectType::YELLOW_PART},
		{ CPoint(250, 355), 0.1f, 0.35f, -45.0f, ObjectType::GREEN_PART },
		{ CPoint(250, 355), 0.1f, 0.35f, 0, ObjectType::GREEN_PART },
		{ CPoint(250, 355), 0.1f, 0.35f, 45.0f, ObjectType::GREEN_PART },
		{ CPoint(195, 305), 0.2f, 0.35f, -90.0f, ObjectType::GREEN_PART },
		{ CPoint(195, 305), 0.2f, 0.35f, 0, ObjectType::GREEN_PART },
		{ CPoint(195, 230), 0.3f, 0.35f, 0, ObjectType::GREEN_PART },
		{ CPoint(300, 305), 0.2f, 0.35f, 90, ObjectType::GREEN_PART },
		{ CPoint(370, 305), 0.2f, 0.35f, 45, ObjectType::GREEN_PART },
		{ CPoint(370, 305), 0.2f, 0.35f, 135, ObjectType::GREEN_PART }
	};
	float all_obj_rot_angle = 0;
	
	const std::vector<CPoint> elipse_coords = {
		{240, 343},
		{185, 295},
		{185, 220},
		{290, 295},
		{362, 295}
	};
	const int elipse_size = 20;
// Implementation
public:
	virtual ~CIND18623View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18623View.cpp
inline CIND18623Doc* CIND18623View::GetDocument() const
   { return reinterpret_cast<CIND18623Doc*>(m_pDocument); }
#endif

