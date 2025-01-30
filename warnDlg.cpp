// warnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "warnDlg.h"


// warnDlg dialog

IMPLEMENT_DYNAMIC(warnDlg, CDialog)
warnDlg::warnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(warnDlg::IDD, pParent)
{
}

warnDlg::~warnDlg()
{
}

void warnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(warnDlg, CDialog)
END_MESSAGE_MAP()


// warnDlg message handlers
