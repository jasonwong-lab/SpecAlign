// alignfitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "alignfitDlg.h"


// alignfitDlg dialog

IMPLEMENT_DYNAMIC(alignfitDlg, CDialog)
alignfitDlg::alignfitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(alignfitDlg::IDD, pParent)
	, fitMethod(1)
	, win(20)
	, segsize(500)
	, lower(0)
	, upper(0)
	, scale(1)
	, ref(0)
{
}

alignfitDlg::~alignfitDlg()
{
}

void alignfitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, win);
	DDX_Text(pDX, IDC_EDIT2, segsize);
	DDX_Text(pDX, IDC_EDIT3, lower);
	DDX_Text(pDX, IDC_EDIT7, upper);
	DDX_Text(pDX, IDC_EDIT4, scale);
	DDX_Text(pDX, IDC_EDIT5, ref);
}


BEGIN_MESSAGE_MAP(alignfitDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// alignfitDlg message handlers

void alignfitDlg::OnBnClickedButton1()
{
	lower = start;
	upper = end;
	UpdateData(FALSE);
}
