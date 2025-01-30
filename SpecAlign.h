// SpecAlign.h : main header file for the SpecAlign application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CSpecAlignApp:
// See SpecAlign.cpp for the implementation of this class
//

class CSpecAlignApp : public CWinApp
{
public:
	CSpecAlignApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
};

extern CSpecAlignApp theApp;