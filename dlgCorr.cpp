// dlgCorr.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "dlgCorr.h"


// dlgCorr dialog

IMPLEMENT_DYNAMIC(dlgCorr, CDialog)
dlgCorr::dlgCorr(CWnd* pParent /*=NULL*/)
	: CDialog(dlgCorr::IDD, pParent)
	, win(40)
{
}

dlgCorr::~dlgCorr()
{
}

void dlgCorr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, win);
}


BEGIN_MESSAGE_MAP(dlgCorr, CDialog)
END_MESSAGE_MAP()


// dlgCorr message handlers
