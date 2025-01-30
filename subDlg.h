#pragma once


// subDlg dialog

class subDlg : public CDialog
{
	DECLARE_DYNAMIC(subDlg)

public:
	subDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~subDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int subVal;
};
