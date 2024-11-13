
// TestZaGDIDrugi.h : main header file for the TestZaGDIDrugi application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTestZaGDIDrugiApp:
// See TestZaGDIDrugi.cpp for the implementation of this class
//

class CTestZaGDIDrugiApp : public CWinApp
{
public:
	CTestZaGDIDrugiApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestZaGDIDrugiApp theApp;
