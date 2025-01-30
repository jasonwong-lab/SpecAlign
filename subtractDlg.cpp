// subtractDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "subtractDlg.h"


// subtractDlg dialog

IMPLEMENT_DYNAMIC(subtractDlg, CDialog)
subtractDlg::subtractDlg(CWnd* pParent /*=NULL*/)
	: CDialog(subtractDlg::IDD, pParent)
	, spec1(1)
	, spec2(2)
{
}

subtractDlg::~subtractDlg()
{
}

void subtractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, spec1);
	DDX_Text(pDX, IDC_EDIT2, spec2);
}


BEGIN_MESSAGE_MAP(subtractDlg, CDialog)
END_MESSAGE_MAP()


// subtractDlg message handlers
