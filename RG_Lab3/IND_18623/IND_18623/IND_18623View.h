
// IND_18623View.h : interface of the CIND18623View class
//

#pragma once
#include<memory>
#include <afxwin.h>  // Ensure MFC headers are included for CDC and CBitmap
#include"DImage.h"
#include<vector>

class CIND18623View : public CView
{
protected: // create from serialization only
	CIND18623View() noexcept;
	DECLARE_DYNCREATE(CIND18623View)

// Attributes
public:
	CIND18623Doc* GetDocument() const;
	XFORM trans_matrix;
	float center_rot_angle;
	bool right_mult = true;
	std::vector<DImage> puzzle_parts;
	const int cols = 3;
	const int rows = 3;
// Operations
public:
	void rotate(CDC *pDC,float angle, bool right_mult);
	void scale(CDC *pDC, float sx, float sy, bool right_mult);
	void translate(CDC* pDC, float dx, float dy, bool right_mult);
	void mirror(CDC* pDC, bool mx, bool my,bool right_mult);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	CBitmap* bitmap_make_transparent(DImage& img, COLORREF transparentColor, CDC* pDC);

	void DrawTransparentImage(DImage& img, CDC* pDC, CRect rcImg, CRect rcDC, COLORREF clrTransparent);

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
};

#ifndef _DEBUG  // debug version in IND_18623View.cpp
inline CIND18623Doc* CIND18623View::GetDocument() const
   { return reinterpret_cast<CIND18623Doc*>(m_pDocument); }
#endif

