// smoothDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "smoothDlg.h"


// smoothDlg dialog

IMPLEMENT_DYNAMIC(smoothDlg, CDialog)
smoothDlg::smoothDlg(CWnd* pParent /*=NULL*/)
	: CDialog(smoothDlg::IDD, pParent)
	, fsize(8)
{
}

smoothDlg::~smoothDlg()
{
}

void smoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, fsize);
	DDV_MinMaxInt(pDX, fsize, 2, 100);
}


BEGIN_MESSAGE_MAP(smoothDlg, CDialog)
END_MESSAGE_MAP()


// smoothDlg message handlers
