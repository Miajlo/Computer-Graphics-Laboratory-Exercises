
// IND_18623View.h : interface of the CIND18623View class
//

#pragma once
#include <string>
#include"transforms.h"
#include<vector>
#include<unordered_map>
#include"cactus_elment.h"
using type_utils::ObjectType;

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
	//virtual BOOL PreTranslateMessage(MSG* msg) override;

	bool do_grid_draw = false;


	std::vector<CPoint> trapezoid = { {210, 500}, {291, 500}, {300, 450}, {200, 450} };
	std::vector<CPoint> rectangle = { {188,450}, {310, 430} };
	const CPoint all_rot_point = { 250, 426 };

	type_utils::element_group all_objects = {
		
		std::vector<type_utils::cactus_element>{
			{CPoint(100, 200), 0.5f, 0.5f, 45.0f, ObjectType::GREEN_PART},
			{CPoint(150, 250), 0.3f, 0.3f, 90.0f, ObjectType::YELLOW_PART},
			{CPoint(200, 300), 1.0f, 1.0f, 30.0f, ObjectType::CIRCLE}
		},
		
		CPoint(120, 220)
	};



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

