#pragma once


// evalDlg dialog

class evalDlg : public CDialog
{
	DECLARE_DYNAMIC(evalDlg)

public:
	evalDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~evalDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG26 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double start;
	double end;
	int segments;
};
