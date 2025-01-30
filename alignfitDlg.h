#pragma once


// alignfitDlg dialog

class alignfitDlg : public CDialog
{
	DECLARE_DYNAMIC(alignfitDlg)

public:
	alignfitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~alignfitDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG17 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Fitting method
	int fitMethod;
	int win;
	int segsize;
	double lower;
	double upper;
	double start;
	double end;
	afx_msg void OnBnClickedButton1();
	float scale;
	int ref;
};
