
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
	float center_rot_angle = 0;
	const CPoint rot_center = { 250, 250 };
	bool right_mult = true;
	bool first_time = true;
	bool do_grid_draw = false;
	const int grid_width = 500;
	const int grid_height = 500;
	const int grid_unit_size = 20;
	const COLORREF GRID_COLOR = RGB(200, 200, 200);

	const int sx = 1, sy = 1;
	std::vector<std::vector<DImage>> puzzle_parts;

	const std::string path_base = "res\\Mon_";
	const std::string file_ext = ".bmp";

	std::vector<std::vector<ImagePart>> imageParts = {
		{
			{109, {-80, -235}, {17, 170}, true, false},
			{17, {-33, -100}, {323, 34}, false, true},
			{115, {-27, -100}, {318, 22}, false, true}
		},
		{
			{ 203, {-165, -37}, {20, 165}, true, false, true},
			{ -32, {-100, -33}, {320, 166}, false, true},
			{ 145, {-140, -240}, {463, 164}, false, true}
		},
		{
			{ 71, {-170, -230}, {170, 463}, false, true},
			{ -18, {-76, -41}, {320, 314}, false, true},
			{ 14, {-215, -189}, {465, 316}, true, false},
		}	
	};
// Operations
public:
	void rotate(CDC *pDC,float angle, bool right_mult);
	void scale(CDC *pDC, float sx, float sy, bool right_mult);
	void translate(CDC* pDC, float dx, float dy, bool right_mult);
	void mirror(CDC* pDC, bool mx, bool my,bool right_mult);
	void draw_img(CDC* pDC);
	void draw_grid(CDC *pDC);

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

