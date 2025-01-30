// alignCombineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "alignCombineDlg.h"


// alignCombineDlg dialog

IMPLEMENT_DYNAMIC(alignCombineDlg, CDialog)
alignCombineDlg::alignCombineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(alignCombineDlg::IDD, pParent)
	, lower(0)
	, upper(0)
	, scale(1)
	, lookahead(1)
	, shift(20)
	, ref(0)
{
}

alignCombineDlg::~alignCombineDlg()
{
}

void alignCombineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, lower);
	DDX_Text(pDX, IDC_EDIT7, upper);
	DDX_Text(pDX, IDC_EDIT2, scale);
	DDX_Text(pDX, IDC_EDIT4, lookahead);
	DDX_Text(pDX, IDC_EDIT1, shift);
	DDX_Text(pDX, IDC_EDIT5, ref);
}


BEGIN_MESSAGE_MAP(alignCombineDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// alignCombineDlg message handlers

void alignCombineDlg::OnBnClickedButton1()
{
	lower = start;
	upper = end;
	UpdateData(FALSE);
}
