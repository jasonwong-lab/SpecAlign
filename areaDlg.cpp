// areaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "areaDlg.h"


// areaDlg dialog

IMPLEMENT_DYNAMIC(areaDlg, CDialog)
areaDlg::areaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(areaDlg::IDD, pParent)
	, percent(68)
{
}

areaDlg::~areaDlg()
{
}

void areaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, percent);
}


BEGIN_MESSAGE_MAP(areaDlg, CDialog)
END_MESSAGE_MAP()


// areaDlg message handlers
