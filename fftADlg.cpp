// fftADlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "fftADlg.h"


// fftADlg dialog

IMPLEMENT_DYNAMIC(fftADlg, CDialog)
fftADlg::fftADlg(CWnd* pParent /*=NULL*/)
	: CDialog(fftADlg::IDD, pParent)
	, low(0)
	, high(0)
	, scale(1)
	, shift(20)
	, lookahead(1)
	, ref(0)
{
}

fftADlg::~fftADlg()
{
}

void fftADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, low);
	DDX_Text(pDX, IDC_EDIT7, high);
	DDX_Text(pDX, IDC_EDIT2, scale);
	DDX_Text(pDX, IDC_EDIT1, shift);
	DDX_Text(pDX, IDC_EDIT4, lookahead);
	DDX_Text(pDX, IDC_EDIT5, ref);
}


BEGIN_MESSAGE_MAP(fftADlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// fftADlg message handlers

void fftADlg::OnBnClickedButton1()
{
	low = start;
	high = end;
	UpdateData(FALSE);
}
