// scaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "scaleDlg.h"


// scaleDlg dialog

IMPLEMENT_DYNAMIC(scaleDlg, CDialog)
scaleDlg::scaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(scaleDlg::IDD, pParent)
	, fact(0.5)
{
}

scaleDlg::~scaleDlg()
{
}

void scaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, fact);
	DDV_MinMaxDouble(pDX, fact, 0.00000000001, 100);
}


BEGIN_MESSAGE_MAP(scaleDlg, CDialog)
END_MESSAGE_MAP()


// scaleDlg message handlers
