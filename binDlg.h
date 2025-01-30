#pragma once


// binDlg dialog

class binDlg : public CDialog
{
	DECLARE_DYNAMIC(binDlg)

public:
	binDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~binDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double binsize;
};
