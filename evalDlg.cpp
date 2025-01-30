// evalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "evalDlg.h"


// evalDlg dialog

IMPLEMENT_DYNAMIC(evalDlg, CDialog)
evalDlg::evalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(evalDlg::IDD, pParent)
	, start(0)
	, end(0)
	, segments(1)
{
}

evalDlg::~evalDlg()
{
}

void evalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, start);
	DDX_Text(pDX, IDC_EDIT2, end);
	DDX_Text(pDX, IDC_EDIT3, segments);
	DDV_MinMaxInt(pDX, segments, 0, 100);
}


BEGIN_MESSAGE_MAP(evalDlg, CDialog)
END_MESSAGE_MAP()


// evalDlg message handlers
