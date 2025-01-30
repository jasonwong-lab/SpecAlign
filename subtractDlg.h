#pragma once


// subtractDlg dialog

class subtractDlg : public CDialog
{
	DECLARE_DYNAMIC(subtractDlg)

public:
	subtractDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~subtractDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG14 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT spec1;
	UINT spec2;
};
