#pragma once


// alignCombineDlg dialog

class alignCombineDlg : public CDialog
{
	DECLARE_DYNAMIC(alignCombineDlg)

public:
	alignCombineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~alignCombineDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG18 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double lower;
	double upper;
	float scale;
	double start;
	double end;
	afx_msg void OnBnClickedButton1();
	int lookahead;
	int shift;
	int ref;
};
