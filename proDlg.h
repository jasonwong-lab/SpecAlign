#pragma once
#include "afxcmn.h"


// proDlg dialog

class proDlg : public CDialog
{
	DECLARE_DYNAMIC(proDlg)

public:
	proDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~proDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl progress;
};
