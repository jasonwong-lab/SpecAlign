#pragma once


// DlgComb dialog

class DlgComb : public CDialog
{
	DECLARE_DYNAMIC(DlgComb)

public:
	DlgComb(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgComb();

// Dialog Data
	enum { IDD = IDD_DIALOG13 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString samples;
};
