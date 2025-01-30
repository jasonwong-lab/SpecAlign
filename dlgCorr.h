#pragma once


// dlgCorr dialog

class dlgCorr : public CDialog
{
	DECLARE_DYNAMIC(dlgCorr)

public:
	dlgCorr(CWnd* pParent = NULL);   // standard constructor
	virtual ~dlgCorr();

// Dialog Data
	enum { IDD = IDD_DIALOG20 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int win;
};
