#pragma once
#include "afxwin.h"


// rescaleDlg dialog

class rescaleDlg : public CDialog
{
	DECLARE_DYNAMIC(rescaleDlg)

public:
	rescaleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~rescaleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG22 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit lower;
	CEdit upper;
	CEdit scale;
	double lower_v;
	double upper_v;
	double scale_v;
	CButton r1;
	bool interpol;

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	virtual INT_PTR DoModal();
	int choice;
	afx_msg void OnBnClickedRadio3();
};
