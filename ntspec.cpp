// ntspec.cpp : implementation file
//

#include "stdafx.h"
#include "SpecAlign.h"
#include "ntspec.h"
#include "sampProp.h"
#include "SpecAlignView.h"

#define sqr(x) ((x)*(x))

#define PI 3.14159265358979323846
#define M_2S2LN2 2.354820045030949382
#define SQ2PI 2.50662827463100050241577
/* These are apparently not defined in MSVC */
#if !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif
#if !defined(M_LN2)
#define M_LN2 0.69314718055994530942
#endif

static double voigt(double a, double r, double loc, double w, double x)
    /* Evaluate normalized Voigt profile at x around xc with Gaussian
     * linewidth contribution w[0] and Lorentzian linewidth
     * contribution w[1].
     */
{
	double lorenz_v = 0;
	double gauss_dev = 0;
	double gauss_v = 0;
	if (r != 0)
		lorenz_v = (fabs(w)/(2*M_PI))/(sqr(x-loc)+sqr(w/2));
	if (r != 1){
		gauss_dev = fabs(w)/M_2S2LN2;
		gauss_v = (1/(gauss_dev*sqrt(2*M_PI)))*exp(-(sqr(x-loc)/(2*sqr(gauss_dev))));
	}
	//double res = lorenz_v;
	double res = ((r*lorenz_v)+((1-r)*gauss_v));
	return 	a*res;
}

static double lognorm(double a, double loc, double r, double w, double x)
{
	if (x <= loc)
		return 0;
	long double exp_v = exp(-fabs(r));
	long double top = sqr(log((x-loc)/w));
	long double mid = 2.0*sqr(exp_v);
	long double bottom = exp_v*(x-loc)*SQ2PI;
	long double res =a*(exp(-top/mid)/bottom);
	return res;
}

// ntspec dialog

IMPLEMENT_DYNAMIC(ntspec, CDialog)
ntspec::ntspec(CWnd* pParent /*=NULL*/)
	: CDialog(ntspec::IDD, pParent)
{
	avgSpec = false;
	m_bminx = -1;
	m_bmaxx = -1;
	m_bminy = -1;
	m_bmaxy = -1;
	created = false;
	plotP = false;
	baseLoc = -1;
}

ntspec::~ntspec()
{
}


void ntspec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, label);
	DDX_Control(pDX, IDC_NTGRAPHCTRL1, spec);
}


BEGIN_MESSAGE_MAP(ntspec, CDialog)
	ON_WM_SIZE()
//	ON_WM_PARENTNOTIFY()
ON_WM_PARENTNOTIFY()
ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
ON_WM_GETMINMAXINFO()
ON_WM_WINDOWPOSCHANGED()
END_MESSAGE_MAP()


void ntspec::setRange(double minx, double maxx, double miny, double maxy)
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

void ntspec::getRange(double &minx, double &maxx, double &miny, double &maxy)
{
	minx = m_minx;
	miny = m_miny;
	maxx = m_maxx;
	maxy = m_maxy;
}

void ntspec::undoRange()
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

void ntspec::revertRange(bool rel)
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

void ntspec::copyRanges(ntspec *o)
{
	m_minx = o->m_minx;
	m_miny = o->m_miny;
	m_maxx = o->m_maxx;
	m_maxy = o->m_maxy;
	m_bminx = o->m_bminx;
	m_bminy = o->m_bminy;
	m_bmaxx = o->m_bmaxx;
	m_bmaxy = o->m_bmaxy;
	m_ominx = o->m_ominx;
	m_ominy = o->m_ominy;
	m_omaxx = o->m_omaxx;
	m_omaxy = o->m_omaxy;
}

double ntspec::getMaxY(double xmin, double xmax, int &startPos)
{

	register int i = 0;
	double maxy = 0;
	register double yval;
	if (startPos == 0){
		while (spec.get_ElementXValue(i,0) < xmin){
			++i;
		}
		startPos = i;
	}
	i = startPos;
	while (spec.get_ElementXValue(i,0) < xmax){
		yval = spec.get_ElementYValue(i,0);
		if (yval > maxy){
			maxy = yval;
		}
		++i;
	}
	return maxy;
}

// ntspec message handlers

void ntspec::OnSize(UINT nType, int cx, int cy)
{
	if (created){
		CRect lRect;
		label.GetWindowRect(&lRect);
		CRect sRect;
		spec.GetWindowRect(&sRect);
		spec.MoveWindow(0,0,cx-80,sRect.Height());
		label.MoveWindow(cx-70,cy-110,lRect.Width(),lRect.Height());
	}
	CDialog::OnSize(nType, cx, cy);
}

void ntspec::OnParentNotify(UINT message, LPARAM lParam)
{
	CDialog::OnParentNotify(message, lParam);

	// TODO: Add your message handler code here
}

void ntspec::updateCropRange()
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

void ntspec::OnBnClickedButton1()
{
	CString thisName;
	label.GetWindowText(thisName);
		//return;
    sampProp pDlg(this);
	if (thisName == "Average"){
		pDlg.position = 0;
		pDlg.smooth =0;
	}
	else
		pDlg.position = d->pos+1;
	pDlg.name = d->name;
	pDlg.cls = d->cls;
	pDlg.desc = d->desc;
	pDlg.linetype = d->lineType;
	if (pDlg.DoModal() == IDOK){
		if (thisName != "Average"){
			char t[256] = "";
			if (pDlg.name != ""){
				if (d->name != pDlg.name){
					d->name = pDlg.name;
					CString newName(itoa(pDlg.position,t,10));
					newName.Append(". ");
					newName.Append(pDlg.name);
					label.SetWindowText(newName);
				}
			}
			d->cls = pDlg.cls;
			d->desc = pDlg.desc;
			d->lineType = pDlg.linetype;
			if (pDlg.position != d->pos+1){
				CSpecAlignView* pView = (CSpecAlignView*) GetParent();
				pView->moveSpec(d->pos,pDlg.position-1);
			}
			if (pDlg.sub){
				d->calBase(pDlg.subval);
				d->subtractBase();
			}
			if (pDlg.ToSmooth){
				d->SGsmooth(pDlg.smooth,d->input);
			}
			if (pDlg.sub || pDlg.ToSmooth){
				if (!d->peaks.empty()){
					d->pickPeaks(d->m_cut,d->m_win,d->m_height);
					drawAnno();
				}
				if (plotP)
					plotPeaks();
				else
					drawSpec(FALSE, fDraw);
			}
		}
		if (pDlg.copy){
			spec.CopyToClipboard();
		}
	}
}

void ntspec::drawSpec(bool reRange, bool fastDraw, int linetype)
{
	fDraw = fastDraw;
	char t[256]="";
	spec.ClearGraph();
	if (ID != -1)
		spec.SetElementLineColor(RGB(0,0,0));
	else
		spec.SetElementLineColor(RGB(0,0,200));
	spec.SetElementLinetype(linetype);
	spec.SetElementWidth(1);
	if (d->name.Find("Combine") >= 0){
		spec.SetElementLineColor(RGB(0,200,0));
	}
	if (ID != -1){
		CString s(itoa(ID+1,t,10));
		s.Append(". ");
		s.Append(d->name);
		label.SetWindowText(s);
	}
	else
		label.SetWindowText(_T("Average"));
	//spectra[i].d = &(pDoc->dat.train.at(i));
	double max_y = -9999999999;
	double min_y = 999999999;
	int incr = 1;
	if (fastDraw){
		if (d->input.size() > 30000)
			incr = 5;
		else if (d->input.size() > 20000)
			incr = 4;
		else if (d->input.size() > 10000)
			incr = 3;
		else if (d->input.size() > 5000)
			incr = 2;
		else
			incr = 1;
	}
	while (d->mz_val->size() > d->input.size())
		d->input.push_back(d->input.back());
	while (d->mz_val->size() < d->input.size())
		d->input.pop_back();
	spec.PlotY(d->mz_val,&d->input,0);

	for (unsigned int j = 0; j < d->input.size(); j++){
//			spec.PlotXY(d->mz_val->at(j),d->input.at(j),0);
		if (d->input.at(j) > max_y)
			max_y = d->input.at(j);
		if (d->input.at(j) < min_y)
			min_y = d->input.at(j);
	}
	if (!d->base.empty()){
		showBase();
	}
	if (!d->peaks.empty()){
		showModels();
	}
	if (min_y > 0)
		min_y = 0;
	if (reRange)
		setRange(d->mz_val->at(0),d->mz_val->back(),min_y*1.15,max_y*1.15);
	plotP = false;
}

void ntspec::drawAnno()
{
	if (d->peaks.empty()){
		spec.DeleteAnnotation(0);
		return;
	}
	int  decimal, sign;
	char *buffer;
	spec.DeleteAnnotation(0);
	for (unsigned int j = 0; j < d->peaks.size(); ++j){
		if (d->peaks.at(j).intn != 0){
			spec.AddAnnotation();
			spec.SetAnnoLabelX(d->peaks.at(j).mz);
			spec.SetAnnoLabelY(d->peaks.at(j).intn);
			spec.SetAnnoLabelBkColor(RGB(255,255,255));
			spec.SetAnnoLabelColor(RGB(0,0,0));
			buffer = _fcvt(d->peaks.at(j).mz, 2, &decimal, &sign );
			CString val(buffer);
			val.Insert(decimal,'.');
			val.Truncate(val.GetLength()-1);
			spec.SetAnnoLabelCaption(CString(val));
		}
	}
}

void ntspec::plotPeaks()
{
	if (d->peaks.empty()){
		spec.ClearGraph();
		return;
	}
	spec.ClearGraph();
	if (ID != -1)
		spec.SetElementLineColor(RGB(0,0,0));
	else
		spec.SetElementLineColor(RGB(200,0,0));
	spec.SetElementLinetype(9);
	spec.SetElementWidth(1);
	//double max_y = 0;
	for (unsigned int j = 0; j < d->peaks.size(); ++j){
		spec.PlotXY(d->peaks.at(j).mz,d->peaks.at(j).intn,0);
	}
	plotP = true;
}

void ntspec::removeBase()
{
	spec.DeleteElement(baseLoc);
	baseLoc = -1;
}

void ntspec::showBase()
{
	if (d->base.size() == 0)
		return;
	int incr;
	if (d->input.size() > 30000)
		incr = 25;
	else if (d->input.size() > 20000)
		incr = 20;
	else if (d->input.size() > 10000)
		incr = 15;
	else if (d->input.size() > 5000)
		incr = 10;
	else
		incr = 5;
	spec.AddElement();
	spec.SetElementLineColor(RGB(200,0,0));
	spec.SetElementLinetype(0);
	spec.SetElementWidth(1);
	while (d->mz_val->size() > d->base.size())
		d->base.push_back(d->base.back());
	while (d->mz_val->size() < d->base.size())
		d->base.pop_back();
	spec.PlotY(d->mz_val,&d->base,spec.GetElementCount()-1);
	//for (unsigned int j = 0; j < d->base.size(); j+=incr){
	//		spec.PlotXY(d->mz_val->at(j),d->base.at(j),spec.GetElementCount()-1);
	//}
	baseLoc = spec.GetElementCount()-1;
}

void ntspec::removeAllModels()
{
	for (short i = 1; i < spec.GetElementCount(); ++ i){
		if (i != baseLoc){
			spec.DeleteElement(i);
			i--;
		}
	}
	if (baseLoc != -1 && baseLoc != 1)
		showBase();
}

bool ntspec::showModels()
{
	bool drawn = false;

	vector <double> tmp_val;
	vector <double> tmp_mz;
	for (unsigned int j = 0; j < d->peaks.size(); j++){
		if (d->peaks.at(j).modelled){
			drawn = true;
			spec.AddElement();
			spec.SetElementLineColor(RGB(0,200,0));
			spec.SetElementLinetype(2);
			spec.SetElementWidth(1);
			double stdev = 0;
			double mean = 0;
			int type = d->peaks.at(j).type;
			stdev = fabs(d->peaks.at(j).w);
			mean = d->peaks.at(j).loc;
			double startc;
			if (d->peaks.at(j).type !=3)
				startc = mean-(stdev*3.5);
			else
				startc = d->peaks.at(j).loc;
			if (startc < 0)
				startc = 0;
			double endc;
			if (d->peaks.at(j).type !=3)
				endc = mean+(stdev*3.5);
			else{
				int end = min(d->mz_val->size()-1,(int)floor(d->peaks.at(j).ebase*1.1));
				endc = d->mz_val->at(end);
			}
			if (endc > d->mz_val->back())
				endc = d->mz_val->back();
			int stpos = 0;
			while (stpos < d->mz_val->size() && d->mz_val->at(stpos) <= startc)
				stpos++;
			int epos = stpos;
			while (epos < d->mz_val->size() && d->mz_val->at(epos) < endc)
				epos++;
			if (epos -stpos < 4){
				stpos -= 2;
				epos +=2;
			}
			if (stpos > 0 && epos < d->mz_val->size()){
				//if (d->peaks.at(j).type != 0){
				//	if (stpos> 0)
				//		spec.PlotXY(d->mz_val->at(stpos-1),d->base.at(stpos-1),spec.GetElementCount()-1);
				//	else
				//		spec.PlotXY(0,d->base.at(stpos),spec.GetElementCount()-1);
				//}
				double base_val;
				int incr = 1;
				if (epos-stpos > 1000)
					incr = (int)(floor)((double)(epos-stpos)/(double)1000);
				
				for (unsigned int k = stpos; k < epos; k+=incr){
					tmp_mz.push_back(d->mz_val->at(k));
					if (d->base.size() != 0)
						base_val = d->base.at(k);
					else
						base_val = 0;
					if (d->peaks.at(j).type == 3)
						tmp_val.push_back(lognorm(d->peaks.at(j).a,d->peaks.at(j).loc,d->peaks.at(j).r,d->peaks.at(j).w,d->mz_val->at(k))+base_val);
					else
						tmp_val.push_back(voigt(d->peaks.at(j).a,d->peaks.at(j).r,d->peaks.at(j).loc,d->peaks.at(j).w,d->mz_val->at(k))+base_val);
				}
				spec.PlotY(&tmp_mz,&tmp_val,spec.GetElementCount()-1);
				tmp_mz.clear();
				tmp_val.clear();
			}
		}

	}

	return drawn;
}
void ntspec::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	lpMMI->ptMaxTrackSize.y = INT_MAX;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void ntspec::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

}
