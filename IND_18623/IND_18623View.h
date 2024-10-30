
// IND_18623View.h : interface of the CIND18623View class
//

#pragma once


class CIND18623View : public CView
{
protected: // create from serialization only
	CIND18623View() noexcept;
	DECLARE_DYNCREATE(CIND18623View)

// Attributes
public:
	CIND18623Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

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

