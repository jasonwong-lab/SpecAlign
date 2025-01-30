#pragma once


// modelDlg dialog

class modelDlg : public CDialog
{
	DECLARE_DYNAMIC(modelDlg)

public:
	modelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~modelDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG23 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int model_type;
};
