// SpecAlignView.cpp : implementation of the CSpecAlignView class
//

#include "stdafx.h"
#include "SpecAlign.h"

#include "SpecAlignDoc.h"
#include "SpecAlignView.h"
#include "ProgressBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpecAlignView

IMPLEMENT_DYNCREATE(CSpecAlignView, CScrollView)

BEGIN_MESSAGE_MAP(CSpecAlignView, CScrollView)
	//{{AFX_MSG_MAP(CSpecAlignView)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
//	ON_WM_LBUTTONDOWN()
ON_WM_PARENTNOTIFY()
//ON_WM_LBUTTONUP()
//ON_WM_NCLBUTTONUP()
//ON_WM_MOUSEMOVE()
ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

// CSpecAlignView construction/destruction

CSpecAlignView::CSpecAlignView()
{
	created = false;
	m_Track = false;
	m_Zoom = false;
	m_Crop = false;
	numSpec = 0;
	tnumSpec = 0;
	vAll = false;
	m_Pick = false;
	m_Rem = false;
	fDraw = true;
}

CSpecAlignView::~CSpecAlignView()
{
}

BOOL CSpecAlignView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CSpecAlignView drawing

void CSpecAlignView::OnDraw(CDC* pDC)
{

}

void CSpecAlignView::OnInitialUpdate()
{

	CSize sizeTotal;
	avg.created = false;

	CRect main;
	GetWindowRect(main);

	sizeTotal.cx = main.Width()-20;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	created = true;
	CScrollView::OnInitialUpdate();
}

void CSpecAlignView::OnUpdate(CView *pSender,LPARAM lHint, CObject *pHint)
{
	CScrollView::OnUpdate(pSender,lHint,pHint);
	updateScroll();
	//drawSpec(true);
}

void CSpecAlignView::viewAllSpec(bool fastDraw)
{
	if (!vAll){
		if (allSpec.created)
			allSpec.ShowWindow(SW_HIDE);
		return;
	}
	if (!allSpec.created){
		//CRect main;
		//GetWindowRect(main);
		allSpec.Create(IDD_DIALOG9,this);
		allSpec.spec.ModifyStyleEx(WS_EX_NOPARENTNOTIFY,0);
		//allSpec.MoveWindow(&main);
		allSpec.created = true;
	}
	allSpec.spec.ClearGraph();
	  
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect main;
	GetClientRect(main);
	allSpec.MoveWindow(&main);
	allSpec.spec.MoveWindow(&main);
	updateScroll();
	if (avg.created)
		avg.ShowWindow(SW_HIDE);
	double maxy = -999999;
	double miny = 99999999;
//	for (int i = pDoc->dat.train.size()-1; i != -1; --i) 
	if (pDoc->dat.avg.input.size() != 0 && pDoc->showAvg){
		//allSpec.spec.AddElement();
		allSpec.spec.SetElementLineColor(RGB(0,0,200));
		allSpec.spec.SetElementLinetype(0);
		if (pDoc->stacked)
			allSpec.spec.SetElementWidth(2);
		else
			allSpec.spec.SetElementWidth(4);
		allSpec.spec.SetElementName(CString("Average"));
		if (pDoc->stacked)
			allSpec.spec.SetElementIdentify(TRUE);
		else
			allSpec.spec.SetElementIdentify(FALSE);
		allSpec.spec.AddElement();
	}
	int num;
	if (pDoc->dat.avg.input.size() == 0 || !pDoc->showAvg)
		num = 0;
	else
		num = 1;
	for (int i = 0; i < pDoc->dat.train.size(); ++i)
	{
		if (i != 0)
			allSpec.spec.AddElement();
		allSpec.spec.SetElementLineColor(RGB(0,0,0));
		if (pDoc->dat.train.at(i).lineType != 0 && pDoc->overlap)
			allSpec.spec.SetElementLinetype(1);
		else
			allSpec.spec.SetElementLinetype(0);
		if (pDoc->stacked)
			allSpec.spec.SetElementWidth(1);
		else
			allSpec.spec.SetElementWidth(1);
		allSpec.spec.SetElementName(pDoc->dat.train.at(i).name);
		if (pDoc->stacked)
			allSpec.spec.SetElementIdentify(TRUE);
		else
			allSpec.spec.SetElementIdentify(FALSE);

		for (unsigned int x = 0; x < pDoc->dat.mz_val.size(); x++) 
		{
			if (pDoc->dat.train.at(i).input.at(x) > maxy)
				maxy = pDoc->dat.train.at(i).input.at(x);
			if (pDoc->dat.train.at(i).input.at(x) < miny)
				miny = pDoc->dat.train.at(i).input.at(x);
		}
		spectra[i].ShowWindow(SW_HIDE);
	}
	if (pDoc->dat.train.size() == 1){
		allSpec.spec.AddElement();
		allSpec.spec.SetElementLineColor(RGB(255,0,0));
		allSpec.spec.SetElementLinetype(0);
		allSpec.spec.SetElementWidth(1);
		allSpec.spec.SetElementIdentify(FALSE);
	}
	allSpec.fullMaxY = maxy;
	if (pDoc->overlap)
		allSpec.fullMaxY *=1.05;
	if (miny < 0)
		miny/=pDoc->dat.train.size();
	allSpec.fullMinY = miny;
	double value;
	CProgressBar pBar(_T("Drawing spectra"),20,pDoc->dat.train.size());
//	for (int i = pDoc->dat.train.size()-1; i != -1; --i)
	if (pDoc->dat.avg.input.size() != 0 && pDoc->showAvg){
		int incr = 1;
		if (fastDraw){
			if (pDoc->dat.mz_val.size() > 30000)
				incr = 5;
			else if (pDoc->dat.mz_val.size() > 20000)
				incr = 4;
			else if (pDoc->dat.mz_val.size() > 10000)
				incr = 3;
			else if (pDoc->dat.mz_val.size() > 5000)
				incr = 2;
			else
				incr = 1;
		}
		if (pDoc->overlap){
			allSpec.spec.PlotY(&pDoc->dat.mz_val,&pDoc->dat.avg.input,0);
		}
		else if (pDoc->stacked){
			for (unsigned int x = 0; x < pDoc->dat.mz_val.size(); x=x+incr) 
			{
				//if (pDoc->stacked){
					value = pDoc->dat.avg.input.at(x)/(pDoc->dat.train.size()+1)+maxy/(pDoc->dat.train.size()+1)*((pDoc->dat.train.size()+1)-1);
					//if (x == 0)
						allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), value, 0);
					//else
					//	allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), value, -1);
				//}
				//else if (pDoc->overlap){

				//	if (x== 0)
				//		allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), pDoc->dat.avg.input.at(x), 0);
				//	else
				//		allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), pDoc->dat.avg.input.at(x), -1);
				//}
			}
		}
	}
	for (int i = 0; i < pDoc->dat.train.size(); ++i)
	{
		pBar.StepIt();
		//if (pDoc->overlap){
		//	if(allSpec.spec.GetElementCount() != 1){
		//		allSpec.spec.AddElement();
		//	}
		//	allSpec.spec.PlotY(&pDoc->dat.mz_val,&pDoc->dat.train.at(i).input,allSpec.spec.GetElementCount()-1);
		//}
		//else if (pDoc->stacked){
			for (unsigned int x = 0; x < pDoc->dat.mz_val.size(); x++) 
			{
				if (pDoc->stacked){
					value = pDoc->dat.train.at(i).input.at(x)/(pDoc->dat.train.size()+num)+maxy/(pDoc->dat.train.size()+num)*((pDoc->dat.train.size()+num)-1-(i+num));
					//if (x == 0)
						allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), value, i+num);
					//else
					//	allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), value, -1);
				}
				else if (pDoc->overlap){
				//	if (x == 0)
						allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), pDoc->dat.train.at(i).input.at(x), i+num); 
				//	else
				//		allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), pDoc->dat.train.at(i).input.at(x), -1); 
					if (pDoc->dat.train.size() == 1 && !pDoc->dat.train.front().base.empty()){
						allSpec.spec.PlotXY(pDoc->dat.mz_val.at(x), pDoc->dat.train.at(i).base.at(x), i+num+1); 
					}
				}
			}

		//}
	}
	pBar.Clear();
	allSpec.setRange(spectra[0].m_minx,spectra[0].m_maxx,miny,maxy);
	allSpec.ShowWindow(SW_SHOWNORMAL);
	if (avg.created)
		avg.ShowWindow(SW_HIDE);
	vAll = true;
}


void CSpecAlignView::drawSpec(bool reRange, bool fastDraw)
{
	if (vAll)
		return;
	if (allSpec.created)
		allSpec.ShowWindow(SW_HIDE);
	fDraw = fastDraw;
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	while (numSpec < pDoc->dat.train.size()){
		createSpec();
	}
	char buf[256]="";
	updateScroll();
	if (!pDoc->dat.train.empty()){
		CProgressBar pBar(_T("Drawing spectra"),20,pDoc->dat.train.size()+pDoc->showAvg);
		if (!pDoc->dat.avg.input.empty()){
			if (!avg.created){
				avg.Create(IDD_DIALOG1,this);
				avg.spec.ModifyStyleEx(WS_EX_NOPARENTNOTIFY,0);
				avg.spec.SetFormatAxisLeft(CString("%.1f"));
				avg.created = true;
			}
			pBar.StepIt();
			avg.ID = -1;
			avg.d = &(pDoc->dat.avg);
			avg.drawSpec(reRange, fastDraw, pDoc->barGraph);
			if (pDoc->showAvg)
				avg.ShowWindow(SW_SHOW);
		}
		char t[256]="";
		for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
			pBar.StepIt();
			pDoc->dat.train.at(i).pos = i;
			spectra[i].ID = i;
			spectra[i].d = &(pDoc->dat.train.at(i));
			spectra[i].drawSpec(reRange, fastDraw, pDoc->barGraph);
			spectra[i].ShowWindow(SW_SHOW);
		}
		lrange = pDoc->dat.mz_val.back()-pDoc->dat.mz_val.at(0);
		pBar.Clear();
	}

}

void CSpecAlignView::drawAnno()
{
	if (vAll)
		return;
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc->dat.train.empty() /*&& !pDoc->dat.train.at(0).peaks.empty()*/){   
		if (!pDoc->dat.avg.input.empty()){
			avg.drawAnno();
		}
		for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
			spectra[i].drawAnno();
		}
	}
}

void CSpecAlignView::drawPeaks()
{
	if (vAll)
		return;
	if (allSpec.created)
		allSpec.ShowWindow(SW_HIDE);
	updateScroll();
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc->dat.train.empty()){
		if (!pDoc->dat.avg.input.empty()){
			avg.plotPeaks();
		}
		for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
			spectra[i].plotPeaks();
		}
	}
}

int CSpecAlignView::updateScroll()
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSize sizeTotal;
	CRect main;
	GetWindowRect(&main);
	if (main.Width() < 600)
		sizeTotal.cx = 600;
	else
		sizeTotal.cx = main.Width()-20;
	if (vAll){
		if (pDoc->dat.train.empty() || !spectra[0].created)
			sizeTotal.cy = 100;
		else{
			sizeTotal.cy =main.Height()-20;
		}
	}
	else{
		if (pDoc->dat.train.empty() || !spectra[0].created)
			sizeTotal.cy = 100;
		else{
			CRect tmp;
			spectra[0].GetWindowRect(&tmp);
			sizeTotal.cy = tmp.Height()* (pDoc->dat.train.size()+pDoc->showAvg);
		}
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
//	GetWindowRect(main);
	return sizeTotal.cy;
}


// CSpecAlignView printing

BOOL CSpecAlignView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation

	return DoPreparePrinting(pInfo);
}

void CSpecAlignView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{

}

void CSpecAlignView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSpecAlignView::OnSize(UINT nType, int cx, int cy)
{
	if(created){
		updatePane(cx);
		updateScroll();	
	}
	CScrollView::OnSize(nType,cx,cy);
}

void CSpecAlignView::updatePane(int cx)
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->dat.train.empty())
		return;
	if (avg.created && !vAll){
		if (pDoc->showAvg)
			avg.ShowWindow(SW_SHOW);
		else
			avg.ShowWindow(SW_HIDE);
	}
	if (cx > 600){
		CPoint cpos = GetScrollPosition();
		CPoint o(0,0);
		ScrollToPosition(o);
		CRect last;
		if (avg.created){
			avg.GetWindowRect(&last);
			avg.MoveWindow(0,0,cx,last.Height(),1);
		}
		else
			spectra[0].GetWindowRect(&last);
		for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
			if (spectra[i].created){
				spectra[i].MoveWindow(0,last.Height()*(i+pDoc->showAvg),cx,last.Height(),1);
			}
		}
		ScrollToPosition(cpos);
	}
	else{
		CPoint cpos = GetScrollPosition();
		CPoint o(0,0);
		ScrollToPosition(o);
		CRect last;
		spectra[0].GetWindowRect(&last);
		if (avg.created){
			avg.GetWindowRect(&last);
			avg.MoveWindow(0,0,600,last.Height(),1);
		}
		else
			spectra[0].GetWindowRect(&last);
		for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
			if (spectra[i].created){
				spectra[i].MoveWindow(0,(long int)last.Height()*(i+pDoc->showAvg),cx,last.Height(),1);
			}
		}
		ScrollToPosition(cpos);		
	}
	if (allSpec.created){
		CRect main;
		GetClientRect(main);
		allSpec.MoveWindow(&main);
		allSpec.spec.MoveWindow(&main);
	}
}

void CSpecAlignView::updateSrn(int cx, int cy)
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->dat.train.empty())
		return;
	CSize size = GetTotalSize();
	CRect main;
	GetWindowRect(&main);
	CRect last;
	spectra[0].GetWindowRect(&last);
	double pos = (double)cy/(double)last.Height();
	int specPos = floor(pos);
	double rem = pos-(double)specPos;
	int ypos = 0;
	while (ypos*last.Height() < main.Height()){
		spectra[specPos].MoveWindow(last.top,-(last.Height()*(1-rem))+last.Height()*ypos,last.Width(),last.Height(),TRUE);
		ypos++;
	}
}

// CSpecAlignView diagnostics

#ifdef _DEBUG
void CSpecAlignView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSpecAlignView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CSpecAlignDoc* CSpecAlignView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpecAlignDoc)));
	return (CSpecAlignDoc*)m_pDocument;
}
#endif //_DEBUG

void CSpecAlignView::DoZoom(CPoint point)
{
	CRectTracker tracker;
	double xmin, xmax, axmin, axmax, scaling, rmin,rmax, offset;
	if (tracker.TrackRubberBand(this, point) )
	{
		CPoint pt1, pt2;
		tracker.m_rect.NormalizeRect ();
		pt1= tracker.m_rect.TopLeft ();
		pt2= tracker.m_rect.BottomRight ();

		xmin = pt1.x;
		xmax = pt2.x;
		if (vAll){
			axmin = (double)allSpec.spec.GetXAxisMinPos();
			axmax = (double)allSpec.spec.GetXAxisMaxPos();
		}
		else{
			axmin = (double)spectra[0].spec.GetXAxisMinPos();
			axmax = (double)spectra[0].spec.GetXAxisMaxPos();
		}
		rmin = spectra[0].spec.GetXMin();
		rmax = spectra[0].spec.GetXMax();
		if (xmin < axmin)
			xmin = axmin;
		if (xmax > axmax)
			xmax = axmax;
		scaling = (rmax-rmin)/(axmax-axmin);
		offset = rmin-(scaling*axmin);
 		CSpecAlignDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		unsigned int startPos, endPos;
		startPos = pDoc->dat.getXIndex((xmin*scaling)+offset);
		endPos = pDoc->dat.getXIndex((xmax*scaling)+offset);
		if (!pDoc->dat.train.empty()){  
			if (m_Rem)
			{
				pDoc->dat.removePeaks((xmin*scaling)+offset,(xmax*scaling)+offset);
				removeModels();
				if (!drawModels())
					pDoc->viewPModel = false;
				if (pDoc->showPeaks)
					drawPeaks();
				drawAnno();
				m_Rem = false;
			}
			else if (m_Crop && endPos-startPos > 2)
			{
				pDoc->dat.cropSpec(startPos,endPos);
				if (!pDoc->dat.avg.input.empty()){
					avg.updateCropRange();
					pDoc->showAvg= true;
				}
				for (unsigned int j = 0; j < pDoc->dat.train.size(); ++j){
					spectra[j].updateCropRange();
				}
				bool vState = vAll;
				vAll = false;
				drawSpec(TRUE, fDraw);
				vAll = vState;
				allSpec.updateCropRange();
				viewAllSpec(fDraw);
				
				m_Crop = false;
			}
			else{
				if (avg.created && !pDoc->dat.avg.input.empty() &&pDoc->dat.avg.input.size() == pDoc->dat.mz_val.size() ){
					avg.setRange((xmin*scaling)+offset,(xmax*scaling)+offset,pDoc->dat.avg.getMinY(startPos,endPos)*1.15,pDoc->dat.avg.getMaxY(startPos,endPos)*1.15);
				}
				double maxy = 0;
				double miny = 99999999;
				double cury;
				for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
					cury = pDoc->dat.train.at(i).getMaxY(startPos,endPos);
					miny = pDoc->dat.train.at(i).getMinY(startPos,endPos);

					spectra[i].setRange((xmin*scaling)+offset,(xmax*scaling)+offset,miny*1.15,cury*1.15);
				}
				if (allSpec.created){
					allSpec.setRange((xmin*scaling)+offset,(xmax*scaling)+offset,allSpec.fullMinY,allSpec.fullMaxY);
				}
			}
		}
	}
}

void CSpecAlignView::DoPan(CPoint point)
{


}

void CSpecAlignView::clearBackups()
{
	allSpec.updateCropRange();
	avg.updateCropRange();
 	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
		spectra[i].updateCropRange();
	}
}

// CSpecAlignView message handlers


void CSpecAlignView::OnParentNotify(UINT message, LPARAM lParam)
{
	CScrollView::OnParentNotify(message, lParam);
	if (message == WM_MBUTTONDOWN){
		return;
	}
	if (message == WM_RBUTTONDOWN){
		CSpecAlignDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc->dat.train.empty()){   
			if (!pDoc->dat.avg.input.empty() && pDoc->showAvg){
				avg.undoRange();
			}
			for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
				spectra[i].undoRange();
			}
			allSpec.undoRange();
		}
	}	
	if (m_Pick){
		if (message == WM_LBUTTONDOWN){
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR3));
			CPoint p(lParam);
			DoPick(p);	
		}
	}
	else if (m_Rem){
		if (message == WM_LBUTTONDOWN){
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR4));
			CPoint p(lParam);
			DoZoom(p);
		}
	}
	else if (m_Zoom && !m_Crop){
		if (message == WM_LBUTTONDOWN){
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
			CPoint p(lParam);
			DoZoom(p);
		}
	}
	else if (m_Crop){
		if (message == WM_LBUTTONDOWN){
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR2));
			CPoint p(lParam);
			DoZoom(p);
		}
	}
//	AfxGetApp()->GetMainWnd()->RedrawWindow();
	return;
}

void CSpecAlignView::createSpec()
{
	int i = numSpec;
	spectra[i].created = false;
	spectra[i].Create(IDD_DIALOG1,this);
	spectra[i].spec.ModifyStyleEx(WS_EX_NOPARENTNOTIFY,0);
	spectra[i].spec.SetFormatAxisLeft(CString("%.1f"));
	spectra[i].created = true;
	CRect last;
	if (i != 0)
		spectra[i-1].GetWindowRect(&last);
	else
		spectra[i].GetWindowRect(&last);
	spectra[i].MoveWindow(0,last.Height()*(i+displayAvg),last.Width(),last.Height());
	numSpec++;
}

void CSpecAlignView::rmSpec(int pos)
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	spectra[pDoc->dat.train.size()].ShowWindow(SW_HIDE);
	for (unsigned int i = pos; i < pDoc->dat.train.size(); ++i){
		spectra[i].copyRanges(&spectra[i+1]);
	}
//	spectra[pDoc->dat.train.size()-1].CloseWindow();
	//tnumSpec--;
}

void CSpecAlignView::moveSpec(int oldp, int newp)
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->dat.moveSpec(oldp,newp) == 0){
		AfxGetApp()->DoMessageBox(_T("New position does not exist"),0,0);
		return;
	}
	else{
		drawSpec(TRUE, fDraw);
		if (pDoc->peaksPicked){
			drawAnno();
		}
	}
}

void CSpecAlignView::setUnStackVisible()
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->showAvg)
		avg.ShowWindow(SW_SHOW);
	for (unsigned int i = 0; i < pDoc->dat.train.size(); ++i){
		spectra[i].ShowWindow(SW_SHOW);
	}
	updateScroll();
	if (allSpec.created)
		allSpec.ShowWindow(SW_HIDE);
}

void CSpecAlignView::createAvg()
{
		avg.Create(IDD_DIALOG1,this);
		avg.spec.ModifyStyleEx(WS_EX_NOPARENTNOTIFY,0);
		avg.spec.SetFormatAxisLeft(CString("%.1f"));
		avg.created = true;
}

void CSpecAlignView::DoPick(CPoint pt)
{
	double axmin, axmax, scaling, rmin,rmax, offset;
	if (vAll){
		axmin = (double)allSpec.spec.GetXAxisMinPos();
		axmax = (double)allSpec.spec.GetXAxisMaxPos();
	}
	else{
		axmin = (double)spectra[0].spec.GetXAxisMinPos();
		axmax = (double)spectra[0].spec.GetXAxisMaxPos();
	}
	rmin = spectra[0].spec.GetXMin();
	rmax = spectra[0].spec.GetXMax();
	scaling = (rmax-rmin)/(axmax-axmin);
	offset = rmin-(scaling*axmin);
	double mz = (pt.x*scaling)+offset;
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->dat.addPeak(mz, true);
	if (pDoc->showPeaks)
		drawPeaks();
	drawAnno();
	m_Pick = false;
}

BOOL CSpecAlignView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{

	if (GetTotalSize().cx >= SHRT_MAX || GetTotalSize().cy >= SHRT_MAX )
	{

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_TRACKPOS;

		//nPos is valid only is scroll code is SB_THUMBTRACK

		if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
		{
			GetScrollInfo(SB_HORZ, &info);
			nPos = info.nTrackPos;
		}
		if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
		{
			GetScrollInfo(SB_VERT, &info);
			nPos = info.nTrackPos;
		}
	}

	
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CSpecAlignView::removeBase()
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->viewBase){
		if (pDoc->showAvg)
			avg.removeBase();
		for (unsigned int i = 0; i < numSpec; ++i){
			spectra[i].removeBase();
		}
	}
}

void CSpecAlignView::drawBase()
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->showAvg)
		avg.showBase();
	for (unsigned int i = 0; i < numSpec; ++i){
		spectra[i].showBase();
	}
}

bool CSpecAlignView::drawModels()
{
	bool drawn = false;
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CProgressBar pBar(_T("Drawing models"),20,pDoc->dat.train.size()+pDoc->showAvg);
	if (pDoc->showAvg){
		pBar.StepIt();
		drawn =avg.showModels();
	}
	for (unsigned int i = 0; i < numSpec; ++i){
		pBar.StepIt();
		drawn =spectra[i].showModels();
	}
	pBar.Clear();
	return drawn;
}

void CSpecAlignView::removeModels()
{
	CSpecAlignDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->showAvg)
		avg.removeAllModels();
	for (unsigned int i = 0; i < numSpec; ++i){
		spectra[i].removeAllModels();
	}
}
void CSpecAlignView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{

	CScrollView::OnGetMinMaxInfo(lpMMI);
}
