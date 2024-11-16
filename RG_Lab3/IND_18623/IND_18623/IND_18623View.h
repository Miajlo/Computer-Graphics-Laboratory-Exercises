
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
	float center_rot_angle = -90;
	const CPoint rot_center = { 250, 250 };
	bool right_mult = true;
	bool first_time = true;
	const int sx = 1, sy = 1;
	std::vector<std::vector<DImage>> puzzle_parts;

	const std::string path_base = "res\\Mon_";
	const std::string file_ext = ".bmp";

	std::vector<std::vector<ImagePart>> imageParts = {
		{
			{109, {-80, -235}, {20, 170}, true, false},
			{17, {-33, -100}, {326, 34}, false, true},
			{115, {-27, -100}, {321, 22}, false, true}
		},
		{
			{ 203, {-165, -37}, {23, 165}, true, false, true},
			{ -32, {-100, -33}, {323, 167}, false, true},
			{ 145, {-140, -240}, {466, 165}, false, true}
		},
		{
			{ 71, {-170, -230}, {173, 464}, false, true},
			{ -18, {-76, -41}, {323, 315}, false, true},
			{ 14, {-215, -189}, {468, 317}, true, false},
		}	
	};
// Operations
public:
	void rotate(CDC *pDC,float angle, bool right_mult);
	void scale(CDC *pDC, float sx, float sy, bool right_mult);
	void translate(CDC* pDC, float dx, float dy, bool right_mult);
	void mirror(CDC* pDC, bool mx, bool my,bool right_mult);
	void draw_img(CDC* pDC);

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

