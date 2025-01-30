// denoiseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "denoiseDlg.h"


// denoiseDlg dialog

IMPLEMENT_DYNAMIC(denoiseDlg, CDialog)
denoiseDlg::denoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(denoiseDlg::IDD, pParent)
	, sens(0.5)
{
}

denoiseDlg::~denoiseDlg()
{
}

void denoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, sens);
	DDV_MinMaxDouble(pDX, sens, 0, 1);
}


BEGIN_MESSAGE_MAP(denoiseDlg, CDialog)
END_MESSAGE_MAP()


// denoiseDlg message handlers
