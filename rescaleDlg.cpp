// rescaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "rescaleDlg.h"


// rescaleDlg dialog

IMPLEMENT_DYNAMIC(rescaleDlg, CDialog)
rescaleDlg::rescaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(rescaleDlg::IDD, pParent)
	, lower_v(0)
	, upper_v(1000)
	, scale_v(1)
	, choice(0)
{

}

rescaleDlg::~rescaleDlg()
{
}

void rescaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, lower);
	DDX_Control(pDX, IDC_EDIT2, upper);
	DDX_Control(pDX, IDC_EDIT3, scale);
	DDX_Text(pDX, IDC_EDIT1, lower_v);
	DDX_Text(pDX, IDC_EDIT2, upper_v);
	DDX_Text(pDX, IDC_EDIT3, scale_v);
	DDX_Control(pDX, IDC_RADIO1, r1);
	DDX_Radio(pDX, IDC_RADIO1, choice);
}


BEGIN_MESSAGE_MAP(rescaleDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio3)
END_MESSAGE_MAP()


// rescaleDlg message handlers

void rescaleDlg::OnBnClickedRadio1()
{
	lower.EnableWindow(FALSE);
	upper.EnableWindow(FALSE);
	scale.EnableWindow(FALSE);
	choice = 0;
}

void rescaleDlg::OnBnClickedRadio2()
{
	lower.EnableWindow(TRUE);
	upper.EnableWindow(TRUE);
	scale.EnableWindow(TRUE);
	choice = 2;
}

INT_PTR rescaleDlg::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DoModal();
}

void rescaleDlg::OnBnClickedRadio3()
{
	lower.EnableWindow(FALSE);
	upper.EnableWindow(FALSE);
	scale.EnableWindow(FALSE);
	choice = 1;
}
