#pragma once


// dlgCrop dialog

class dlgCrop : public CDialog
{
	DECLARE_DYNAMIC(dlgCrop)

public:
	dlgCrop(CWnd* pParent = NULL);   // standard constructor
	virtual ~dlgCrop();

// Dialog Data
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double lo;
	double up;
};
