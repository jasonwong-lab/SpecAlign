#pragma once


// peakDlg dialog

class peakDlg : public CDialog
{
	DECLARE_DYNAMIC(peakDlg)

public:
	peakDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~peakDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_cutoff;
	UINT m_win;
	double m_height;
};
