#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// selectSpecDlg dialog

class selectSpecDlg : public CDialog
{
	DECLARE_DYNAMIC(selectSpecDlg)

public:
	selectSpecDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~selectSpecDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CString rspec;
};
