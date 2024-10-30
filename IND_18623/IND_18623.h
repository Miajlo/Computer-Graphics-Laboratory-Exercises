
// IND_18623.h : main header file for the IND_18623 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18623App:
// See IND_18623.cpp for the implementation of this class
//

class CIND18623App : public CWinApp
{
public:
	CIND18623App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18623App theApp;
