// proDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "proDlg.h"


// proDlg dialog

IMPLEMENT_DYNAMIC(proDlg, CDialog)
proDlg::proDlg(CWnd* pParent /*=NULL*/)
	: CDialog(proDlg::IDD, pParent)
{
	Create(proDlg::IDD,NULL);
//	progress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,CRect(10,10,200,30), this,1);
	progress.SetRange32( 0, 100);
	progress.SetStep(1);
}

proDlg::~proDlg()
{
}

void proDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, progress);
}


BEGIN_MESSAGE_MAP(proDlg, CDialog)
END_MESSAGE_MAP()


// proDlg message handlers
