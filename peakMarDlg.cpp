// peakMarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "peakMarDlg.h"


// peakMarDlg dialog

IMPLEMENT_DYNAMIC(peakMarDlg, CDialog)
peakMarDlg::peakMarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(peakMarDlg::IDD, pParent)
	, margin(20)
	, lower(0)
	, upper(0)
{
}

peakMarDlg::~peakMarDlg()
{
}

void peakMarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, margin);
	DDX_Text(pDX, IDC_EDIT2, lower);
	DDX_Text(pDX, IDC_EDIT3, upper);
	DDX_Control(pDX, IDC_EDIT2, clower);
	DDX_Control(pDX, IDC_EDIT3, cupper);
}


BEGIN_MESSAGE_MAP(peakMarDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// peakMarDlg message handlers

void peakMarDlg::OnBnClickedButton1()
{
	lower = start;
	upper = end;
	UpdateData(FALSE);
}
