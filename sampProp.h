#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// sampProp dialog

class sampProp : public CDialog
{
	DECLARE_DYNAMIC(sampProp)

public:
	sampProp(CWnd* pParent = NULL);   // standard constructor
	virtual ~sampProp();

// Dialog Data
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int position;
	CString name;
	int cls;
	CString desc;
	int smooth;

	BOOL ToSmooth;
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnSetfocusEdit5();
	CButton SetSmooth;
	BOOL copy;
	CEdit cpos;
	CEdit cname;
	CEdit ccls;
	CEdit cdesc;
	CEdit cfilter;
	BOOL sub;
	CSliderCtrl mslide;
	int subval;
	CButton SetSub;
	int linetype;
	afx_msg void OnEnChangeEdit7();
};
