// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "SpecAlign.h"
#include "SpecAlignView.h"
#include "SpecAlignDoc.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()
int CChildFrame::m_nDefCmdShow = SW_SHOWMAXIMIZED; 
int CChildFrame::m_nDefCmdShowOld = SW_SHOWMAXIMIZED; 

static TCHAR BASED_CODE szSec[] = _T("Settings"); 
static TCHAR BASED_CODE szShowCmd[] = _T("ShowCmd"); 

void CChildFrame::ActivateFrame(int nCmdShow) 
{ 
	if (nCmdShow == -1) 
	nCmdShow = m_nDefCmdShow; // use our default 
	CMDIChildWnd::ActivateFrame(nCmdShow); 
} 

void CChildFrame::Initialize() 
{ 
	m_nDefCmdShow = AfxGetApp()->GetProfileInt(szSec, szShowCmd, m_nDefCmdShow); 
	m_nDefCmdShowOld = m_nDefCmdShow; 
} 

void CChildFrame::Terminate() 
{ 
	if (m_nDefCmdShow != m_nDefCmdShowOld) 
	{ 
		AfxGetApp()->WriteProfileInt(szSec, szShowCmd, m_nDefCmdShow); 
		m_nDefCmdShowOld = m_nDefCmdShow; 
	} 
} 

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers



void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}
