// fullView.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "fullView.h"


// fullView dialog

IMPLEMENT_DYNAMIC(fullView, CDialog)
fullView::fullView(CWnd* pParent /*=NULL*/)
	: CDialog(fullView::IDD, pParent)
{
	created = false;
	m_bminx = -1;
}

fullView::~fullView()
{
}

void fullView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NTGRAPHCTRL1, spec);
}

void fullView::setRange(double minx, double maxx, double miny, double maxy)
{
	if (m_bminx == -1){
		m_bminx = minx;
		m_bmaxx = maxx;
		m_bminy = miny;
		m_bmaxy = maxy;
	}
	m_ominx.push_back(m_minx);
	m_ominy.push_back(m_miny);
	m_omaxx.push_back(m_maxx);
	m_omaxy.push_back(m_maxy);
	m_minx = minx;
	m_miny = miny;
	m_maxx = maxx;
	m_maxy = maxy;
	//m_bminy = miny;
	//m_bmaxy = maxy;

	spec.SetRange(minx,maxx,miny,maxy);
	
}

void fullView::getRange(double &minx, double &maxx, double &miny, double &maxy)
{
	minx = m_minx;
	miny = m_miny;
	maxx = m_maxx;
	maxy = m_maxy;
}

void fullView::undoRange()
{
	if (m_ominx.size() > 1){
		m_minx = m_ominx.back();
		m_miny = m_ominy.back();
		m_maxx = m_omaxx.back();
		m_maxy = m_omaxy.back();
		m_ominx.pop_back();
		m_ominy.pop_back();
		m_omaxx.pop_back();
		m_omaxy.pop_back();	

		spec.SetRange(m_minx,m_maxx,m_miny,m_maxy);
	}
}

void fullView::revertRange(bool rel)
{
	m_minx = m_bminx;
	m_miny = m_bminy;
	m_maxx = m_bmaxx;
	m_maxy = m_bmaxy;
	m_ominx.resize(1,m_minx);
	m_ominy.resize(1,m_miny);
	m_omaxx.resize(1,m_maxx);
	m_omaxy.resize(1,m_maxy);

	if (!rel)
		spec.SetRange(m_minx,m_maxx,m_miny,m_maxy);
	else
		spec.SetRange(m_minx,m_maxx,m_miny,1.15);
}

void fullView::updateCropRange()
{
	m_bminx = -1;
	m_bmaxx = -1;
	m_bminy = -1;
	m_bmaxy = -1;
	m_ominx.clear();
	m_ominy.clear();
	m_omaxx.clear();
	m_omaxy.clear();
}



BEGIN_MESSAGE_MAP(fullView, CDialog)
END_MESSAGE_MAP()


// fullView message handlers
