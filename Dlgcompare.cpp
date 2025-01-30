// Dlgcompare.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "Dlgcompare.h"


// Dlgcompare dialog

IMPLEMENT_DYNAMIC(Dlgcompare, CDialog)
Dlgcompare::Dlgcompare(CWnd* pParent /*=NULL*/)
	: CDialog(Dlgcompare::IDD, pParent)
	, spec1(1)
	, spec2(2)
	, win_size(500)
	, EU(TRUE)
	, SI(TRUE)
	, CC(TRUE)
	, SCA(TRUE)
	, SCAW(TRUE)
{
}

Dlgcompare::~Dlgcompare()
{
}

void Dlgcompare::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, spec1);
	DDX_Text(pDX, IDC_EDIT3, spec2);
	DDX_Text(pDX, IDC_EDIT5, win_size);
	DDX_Check(pDX, IDC_CHECK1, EU);
	DDX_Check(pDX, IDC_CHECK2, SI);
	DDX_Check(pDX, IDC_CHECK4, CC);
	DDX_Check(pDX, IDC_CHECK3, SCA);
	DDX_Check(pDX, IDC_CHECK5, SCAW);
	DDX_Control(pDX, IDC_EDIT2, s1);
	DDX_Control(pDX, IDC_EDIT3, s2);
}


BEGIN_MESSAGE_MAP(Dlgcompare, CDialog)
END_MESSAGE_MAP()


// Dlgcompare message handlers
