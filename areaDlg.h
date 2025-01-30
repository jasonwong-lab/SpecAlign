#pragma once


// areaDlg dialog

class areaDlg : public CDialog
{
	DECLARE_DYNAMIC(areaDlg)

public:
	areaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~areaDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG24 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float percent;
};
