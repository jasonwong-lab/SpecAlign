#pragma once
#include "afxwin.h"


// Dlgcompare dialog

class Dlgcompare : public CDialog
{
	DECLARE_DYNAMIC(Dlgcompare)

public:
	Dlgcompare(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dlgcompare();

// Dialog Data
	enum { IDD = IDD_DIALOG15 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT spec1;
	UINT spec2;
	UINT win_size;
	BOOL EU;
	BOOL SI;
	BOOL CC;
	BOOL SCA;
	BOOL SCAW;
	CEdit s1;
	CEdit s2;
};
