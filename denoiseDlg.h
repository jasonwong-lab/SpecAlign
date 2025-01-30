#pragma once


// denoiseDlg dialog

class denoiseDlg : public CDialog
{
	DECLARE_DYNAMIC(denoiseDlg)

public:
	denoiseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~denoiseDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG25 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double sens;
};
