// modelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "modelDlg.h"


// modelDlg dialog

IMPLEMENT_DYNAMIC(modelDlg, CDialog)
modelDlg::modelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(modelDlg::IDD, pParent)
	, model_type(1)
{
}

modelDlg::~modelDlg()
{
}

void modelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, model_type);
}


BEGIN_MESSAGE_MAP(modelDlg, CDialog)
END_MESSAGE_MAP()


// modelDlg message handlers
