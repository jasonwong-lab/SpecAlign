#pragma once


// fftADlg dialog

class fftADlg : public CDialog
{
	DECLARE_DYNAMIC(fftADlg)

public:
	fftADlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~fftADlg();

// Dialog Data
	enum { IDD = IDD_DIALOG21 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double low;
	double high;
	float scale;
	double start;
	double end;
	afx_msg void OnBnClickedButton1();
	int shift;
	int lookahead;
	int ref;
};
