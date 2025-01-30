// resCompDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "resCompDlg.h"


// resCompDlg dialog

IMPLEMENT_DYNAMIC(resCompDlg, CDialog)
resCompDlg::resCompDlg(CWnd* pParent /*=NULL*/)
	: CDialog(resCompDlg::IDD, pParent)
{
	results = NULL;
	lsize = 0;
}

resCompDlg::~resCompDlg()
{
}

void resCompDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, results);
}


BEGIN_MESSAGE_MAP(resCompDlg, CDialog)
END_MESSAGE_MAP()


// resCompDlg message handlers

BOOL resCompDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect sz;
	GetClientRect(sz);
	sz.DeflateRect(5,5);

	results = (CListCtrl*)GetDlgItem(IDC_LIST1);
	results->SetExtendedStyle(LVS_EX_GRIDLINES);
	int x = sz.Width()-30;
	results->InsertColumn(0,"Method",LVCFMT_LEFT,x/2);
	results->InsertColumn(1,"Value",LVCFMT_LEFT,500);

	LV_ITEM lv;
	int  decimal, sign;
	char *buffer;
	CString s;
	LPTSTR tmp;
	for (int i = 0; i < lsize; ++i){
		buffer = _fcvt( res[i], 7, &decimal, &sign );
		s.SetString(buffer);
		if (decimal > 0)
			s.Insert(decimal,'.');
		else{
			for (int i = decimal; i< 1; ++i){
				s.Insert(0,'0');
			}
			s.Insert(1,'.');
		}
		if (sign != 0)
			s.Insert(0,'-');
		lv.mask = LVIF_TEXT;
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = label[i];
		results->InsertItem(&lv);
		lv.iSubItem = 1;
		tmp = s.GetBuffer(s.GetLength());
		lv.pszText = tmp;
		results->SetItem(&lv);
	}

	//for (unsigned int j = 0; j < lsize; ++j){
	//	results->InsertItem(&lvitems[j]);
	//	results->SetItem(&lvitems[j]);
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
