// peakDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "peakDlg.h"


// peakDlg dialog

IMPLEMENT_DYNAMIC(peakDlg, CDialog)
peakDlg::peakDlg(CWnd* pParent /*=NULL*/)
	: CDialog(peakDlg::IDD, pParent)
	, m_cutoff(0.01)
	, m_win(21)
	, m_height(2)
{
}

peakDlg::~peakDlg()
{
}

void peakDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cutoff);
	DDV_MinMaxDouble(pDX, m_cutoff, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_win);
	DDX_Text(pDX, IDC_EDIT3, m_height);
	DDV_MinMaxDouble(pDX, m_height, 1, 100);
}


BEGIN_MESSAGE_MAP(peakDlg, CDialog)
END_MESSAGE_MAP()


// peakDlg message handlers
