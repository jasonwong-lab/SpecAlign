// DlgComb.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "DlgComb.h"


// DlgComb dialog

IMPLEMENT_DYNAMIC(DlgComb, CDialog)
DlgComb::DlgComb(CWnd* pParent /*=NULL*/)
	: CDialog(DlgComb::IDD, pParent)
	, samples(_T(""))
{
}

DlgComb::~DlgComb()
{
}

void DlgComb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, samples);
}


BEGIN_MESSAGE_MAP(DlgComb, CDialog)
END_MESSAGE_MAP()


// DlgComb message handlers
