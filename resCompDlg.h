#pragma once
#include "afxwin.h"
#include "afxcmn.h"

//#include <vector>

// resCompDlg dialog

class resCompDlg : public CDialog
{
	DECLARE_DYNAMIC(resCompDlg)

public:
	resCompDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~resCompDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG16 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CListCtrl* results;
	double res[10];
	LPTSTR label[10];
	int lsize;
	virtual BOOL OnInitDialog();
};
