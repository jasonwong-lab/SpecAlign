// alignChoiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "alignChoiceDlg.h"


// alignChoiceDlg dialog

IMPLEMENT_DYNAMIC(alignChoiceDlg, CDialog)
alignChoiceDlg::alignChoiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(alignChoiceDlg::IDD, pParent)
	, choice(3)
{
}

alignChoiceDlg::~alignChoiceDlg()
{
}

void alignChoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, choice);
}


BEGIN_MESSAGE_MAP(alignChoiceDlg, CDialog)
END_MESSAGE_MAP()


// alignChoiceDlg message handlers
