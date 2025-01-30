#pragma once
#include "afxwin.h"


// peakMarDlg dialog

class peakMarDlg : public CDialog
{
	DECLARE_DYNAMIC(peakMarDlg)

public:
	peakMarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~peakMarDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int margin;
	double lower;
	double upper;
	double start;
	double end;
	afx_msg void OnBnClickedButton1();
	CEdit clower;
	CEdit cupper;
};
