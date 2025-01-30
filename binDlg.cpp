// binDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "binDlg.h"


// binDlg dialog

IMPLEMENT_DYNAMIC(binDlg, CDialog)
binDlg::binDlg(CWnd* pParent /*=NULL*/)
	: CDialog(binDlg::IDD, pParent)
	, binsize(20)
{
}

binDlg::~binDlg()
{
}

void binDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, binsize);
}


BEGIN_MESSAGE_MAP(binDlg, CDialog)
END_MESSAGE_MAP()


// binDlg message handlers
