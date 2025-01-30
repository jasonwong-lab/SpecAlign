#pragma once


// scaleDlg dialog

class scaleDlg : public CDialog
{
	DECLARE_DYNAMIC(scaleDlg)

public:
	scaleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~scaleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double fact;
};
