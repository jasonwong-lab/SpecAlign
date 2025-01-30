// sampProp.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "sampProp.h"
#include "ntspec.h"


// sampProp dialog

IMPLEMENT_DYNAMIC(sampProp, CDialog)
sampProp::sampProp(CWnd* pParent /*=NULL*/)
	: CDialog(sampProp::IDD, pParent)
	, position(0)
	, name(_T(""))
	, cls(0)
	, desc(_T(""))
	, smooth(8)
	, ToSmooth(FALSE)
	, copy(FALSE)
	, sub(FALSE)
	, subval(20)
	, linetype(0)
{
}

sampProp::~sampProp()
{
}

void sampProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, position);
	DDX_Text(pDX, IDC_EDIT1, name);
	DDX_Text(pDX, IDC_EDIT3, cls);
	DDX_Text(pDX, IDC_EDIT4, desc);
	DDX_Text(pDX, IDC_EDIT5, smooth);
	DDV_MinMaxInt(pDX, smooth, 3, 50);
	DDX_Check(pDX, IDC_CHECK1, ToSmooth);
	DDX_Control(pDX, IDC_CHECK1, SetSmooth);
	DDX_Check(pDX, IDC_CHECK2, copy);
	DDX_Control(pDX, IDC_EDIT2, cpos);
	DDX_Control(pDX, IDC_EDIT1, cname);
	DDX_Control(pDX, IDC_EDIT3, ccls);
	DDX_Control(pDX, IDC_EDIT4, cdesc);
	DDX_Control(pDX, IDC_EDIT5, cfilter);
	DDX_Check(pDX, IDC_CHECK3, sub);
	DDX_Control(pDX, IDC_CHECK3, SetSub);
	DDX_Text(pDX, IDC_EDIT6, linetype);
	DDX_Text(pDX, IDC_EDIT7, subval);
}


BEGIN_MESSAGE_MAP(sampProp, CDialog)
	ON_EN_CHANGE(IDC_EDIT5, OnEnChangeEdit5)
	ON_EN_SETFOCUS(IDC_EDIT5, OnEnSetfocusEdit5)
	ON_EN_CHANGE(IDC_EDIT7, OnEnChangeEdit7)
END_MESSAGE_MAP()



void sampProp::OnEnChangeEdit5()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	ToSmooth = true;
	SetSmooth.SetCheck(1);
	UpdateData();
}

void sampProp::OnEnSetfocusEdit5()
{
	//ToSmooth = true;
	//SetSmooth.SetCheck(1);
}



void sampProp::OnEnChangeEdit7()
{
	sub = true;
	SetSub.SetCheck(1);
	UpdateData();
}
