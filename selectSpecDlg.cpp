// selectSpecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "selectSpecDlg.h"


// selectSpecDlg dialog

IMPLEMENT_DYNAMIC(selectSpecDlg, CDialog)
selectSpecDlg::selectSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(selectSpecDlg::IDD, pParent)

	, rspec(_T(""))
{
}

selectSpecDlg::~selectSpecDlg()
{
}


void selectSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, rspec);
}


BEGIN_MESSAGE_MAP(selectSpecDlg, CDialog)

END_MESSAGE_MAP()


// selectSpecDlg message handlers



