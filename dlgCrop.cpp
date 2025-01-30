// dlgCrop.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "dlgCrop.h"


// dlgCrop dialog

IMPLEMENT_DYNAMIC(dlgCrop, CDialog)
dlgCrop::dlgCrop(CWnd* pParent /*=NULL*/)
	: CDialog(dlgCrop::IDD, pParent)
	, lo(0)
	, up(0)
{
}

dlgCrop::~dlgCrop()
{
}

void dlgCrop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, lo);
	DDX_Text(pDX, IDC_EDIT2, up);
}


BEGIN_MESSAGE_MAP(dlgCrop, CDialog)
END_MESSAGE_MAP()


// dlgCrop message handlers
