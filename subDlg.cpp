// subDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "subDlg.h"


// subDlg dialog

IMPLEMENT_DYNAMIC(subDlg, CDialog)
subDlg::subDlg(CWnd* pParent /*=NULL*/)
	: CDialog(subDlg::IDD, pParent)
	, subVal(25)
{
}

subDlg::~subDlg()
{
}

void subDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, subVal);
}


BEGIN_MESSAGE_MAP(subDlg, CDialog)
END_MESSAGE_MAP()


// subDlg message handlers
