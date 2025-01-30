#pragma once


// warnDlg dialog

class warnDlg : public CDialog
{
	DECLARE_DYNAMIC(warnDlg)

public:
	warnDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~warnDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
