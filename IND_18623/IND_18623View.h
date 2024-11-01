
// IND_18623View.h : interface of the CIND18623View class
//

#pragma once
#include <string>
#include"file_utils.h"
#include"transforms.h"
#include<vector>
#include<unordered_map>

class CIND18623View : public CView
{
protected: // create from serialization only
	CIND18623View() noexcept;
	DECLARE_DYNCREATE(CIND18623View)
	
// Attributes
public:
	CIND18623Doc* GetDocument() const;
	void draw_trapezoid(CDC* pDC, CPoint position, int v_side, int h_side);
// Operations
public:
	void draw_grid(CDC *pDC, int &grid_width, int &grid_height, int &grid_unit_size);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreTranslateMessage(MSG* msg) override;

	bool do_grid_draw = false;
	std::string yellow_part_name = "cactus_part_light.emf";
	std::string green_part_name = "cactus_part.emf";
	std::string green_part_path;
	std::string yellow_part_path;

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
	
};

#ifndef _DEBUG  // debug version in IND_18623View.cpp
inline CIND18623Doc* CIND18623View::GetDocument() const
   { return reinterpret_cast<CIND18623Doc*>(m_pDocument); }
#endif

