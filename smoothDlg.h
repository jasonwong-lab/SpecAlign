#pragma once


// smoothDlg dialog

class smoothDlg : public CDialog
{
	DECLARE_DYNAMIC(smoothDlg)

public:
	smoothDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~smoothDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int fsize;
};
