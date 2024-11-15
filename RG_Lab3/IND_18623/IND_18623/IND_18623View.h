
// IND_18623View.h : interface of the CIND18623View class
//

#pragma once
#include<memory>
#include <afxwin.h>  // Ensure MFC headers are included for CDC and CBitmap
#include"DImage.h"
#include<vector>
#include <string>
#include<sstream>


struct ImagePart {
	CString imagePath;
	double rotationAngle;   // Rotation angle
	CPoint translate1;      // First translation
	CPoint translate2;      // Second translation
	bool mirrorX;           // Mirror along X-axis
	bool mirrorY;           // Mirror along Y-axis
	bool blue_filter = false;
};


class CIND18623View : public CView
{
protected: // create from serialization only
	CIND18623View() noexcept;
	DECLARE_DYNCREATE(CIND18623View)

// Attributes
public:
	CIND18623Doc* GetDocument() const;
	XFORM trans_matrix;
	float center_rot_angle = -90;
	const CPoint rot_center = { 250, 250 };
	bool right_mult = true;
	bool first_time = true;
	std::vector<std::vector<DImage>> puzzle_parts;
	const int cols = 3;
	const int rows = 3;
	const std::string bitmaps_base = "res\\Mon_";
	std::vector<ImagePart> imageParts = {
    {_T("res\\Mon_0_0.bmp"), 109, {-80, -235}, {20, 170}, true, false},
    {_T("res\\Mon_0_1.bmp"), 17, {-33, -100}, {326, 34}, false, true},
    {_T("res\\Mon_0_2.bmp"), 115, {-27, -100}, {321, 22}, false, true},
    {_T("res\\Mon_1_0.bmp"), 203, {-165, -37}, {23, 165}, true, false, true},
    {_T("res\\Mon_1_1.bmp"), -32, {-100, -33}, {323, 167}, false, true},
    {_T("res\\Mon_1_2.bmp"), 145, {-140, -240}, {466, 165}, false, true},
    {_T("res\\Mon_2_0.bmp"), 71, {-170, -230}, {173, 464}, false, true},
    {_T("res\\Mon_2_1.bmp"), -18, {-76, -41}, {323, 315}, false, true},
    {_T("res\\Mon_2_2.bmp"), 14, {-215, -189}, {468, 317}, true, false},
	};
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

	void DrawTransparentImage(DImage& img, CDC* pDC, CRect &rcImg, CRect &rcDC, COLORREF &clrTransparent, const bool &blue_filter);

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

