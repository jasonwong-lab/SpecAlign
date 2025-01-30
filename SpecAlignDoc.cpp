// SpecAlignDoc.cpp : implementation of the CSpecAlignDoc class
//

#include "stdafx.h"
#include "SpecAlign.h"

#include "SpecAlignDoc.h"
#include "SpecAlignView.h"
#include "ProgressBar.h"

#include "peakDlg.h"
#include "peakMarDlg.h"
#include "selectSpecDlg.h"
#include "FolderDlg.h"
#include "binDlg.h"
#include "smoothDlg.h"
#include "scaleDlg.h"
#include "subDlg.h"
#include "warnDlg.h"
#include "dlgCrop.h"
#include "DlgComb.h"
#include "subtractDlg.h"
#include "Dlgcompare.h"
#include "resCompDlg.h"
#include "alignfitDlg.h"
#include "alignCombineDlg.h"
#include "alignChoiceDlg.h"
#include "dlgCorr.h"
#include "fftADlg.h"
#include "modelDlg.h"
#include "areaDlg.h"
#include "denoiseDlg.h"
#include "evalDlg.h"

#include "XZip.h"
#include "XUnzip.h"

#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpecAlignDoc

IMPLEMENT_DYNCREATE(CSpecAlignDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpecAlignDoc, CDocument)
	ON_COMMAND(ID_IMPORT_FILE, OnImportFile)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_PROCESSING_NORMALIZETIC, OnProcessingNormalizetic)
	ON_COMMAND(ID_EDIT_REVERT, OnEditRevert)
	ON_COMMAND(ID_PROCESSING_RELATIVEINTENSITIES, OnProcessingRelativeintensities)
	ON_COMMAND(ID_VIEW_AVERAGESPEC, OnViewAveragespec)
	ON_UPDATE_COMMAND_UI(ID_VIEW_AVERAGESPEC, OnUpdateViewAveragespec)
	ON_COMMAND(ID_GRAPH_TRACK, OnGraphTrack)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_TRACK, OnUpdateGraphTrack)
	ON_COMMAND(ID_GRAPH_ZOOM, OnGraphZoom)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_ZOOM, OnUpdateGraphZoom)
	ON_COMMAND(ID_EDIT_UNDOZOOM, OnEditUndoZoom)
	ON_COMMAND(ID_EDIT_REVERTALLZOOM, OnEditRevertallzoom)
	ON_COMMAND(ID_VIEWAS_LINEGRAPH, OnViewasLinegraph)
	ON_UPDATE_COMMAND_UI(ID_VIEWAS_LINEGRAPH, OnUpdateViewasLinegraph)
	ON_COMMAND(ID_VIEWAS_BARGRAPH, OnViewasBargraph)
	ON_UPDATE_COMMAND_UI(ID_VIEWAS_BARGRAPH, OnUpdateViewasBargraph)
	ON_COMMAND(ID_SPECTRUM_PICKPEAKS, OnSpectrumPickpeaks)
	ON_COMMAND(ID_SPECTRUM_REMOVEPEAKS, OnSpectrumRemovepeaks)
	ON_COMMAND(ID_PROCESSING_ALIGNPEAKS, OnProcessingAlignpeaks)
	ON_COMMAND(ID_SPECTRUM_PLOTPEAKS, OnSpectrumPlotpeaks)
	ON_UPDATE_COMMAND_UI(ID_SPECTRUM_PLOTPEAKS, OnUpdateSpectrumPlotpeaks)
	ON_COMMAND(ID_IMPORT_FOLDER, OnImportFolder)
	ON_COMMAND(ID_SPECTRUM_CROP, OnSpectrumCrop)
	ON_COMMAND(ID_EXPORT_FILE, OnExportFile)
	ON_COMMAND(ID_SPECTRUM_REMOVESPECTRUM, OnSpectrumRemovespectrum)
	ON_COMMAND(ID_EXPORT_MULTIFILE, OnExportMultifile)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_SPECTRUM_CROP, OnUpdateSpectrumCrop)
	ON_COMMAND(ID_PROCESSINGBINS, OnProcessingbins)
	ON_COMMAND(ID_PROCESSING_SMOOTH, OnProcessingSmooth)
	ON_COMMAND(ID_SPECTRUM_GENERATEAVERAGESPECTRUM, OnSpectrumGenerateaveragespectrum)
	ON_COMMAND(ID_EXPORT_AVERAGESPECTRUM, OnExportAveragespectrum)
	ON_COMMAND(ID_IMPORT_AVERAGESPECTRUM, OnImportAveragespectrum)
	ON_COMMAND(ID_VIEWAS_STACKEDGRAPH, OnViewasStackedgraph)
	ON_UPDATE_COMMAND_UI(ID_VIEWAS_STACKEDGRAPH, OnUpdateViewasStackedgraph)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_SPECTRUM_MERGEWITHAVERAGESPECTRUM, OnSpectrumMergewithaveragespectrum)
	ON_COMMAND(ID_PROCESSING_SCALE, OnProcessingScale)
	ON_COMMAND(ID_PROCESSING_SUBTRACTBASELINE, OnProcessingSubtractbaseline)
	ON_COMMAND(ID_SPECTRUM_MANUALPICKPEAKS, OnSpectrumManualpickpeaks)
	ON_UPDATE_COMMAND_UI(ID_SPECTRUM_MANUALPICKPEAKS, OnUpdateSpectrumManualpickpeaks)
	ON_COMMAND(ID_SPECTRUM_AUTOSCALE, OnSpectrumAutoscale)
	ON_COMMAND(ID_SPECTRUM_MANUALREMOVEPEAKS, OnSpectrumManualremovepeaks)
	ON_UPDATE_COMMAND_UI(ID_SPECTRUM_MANUALREMOVEPEAKS, OnUpdateSpectrumManualremovepeaks)
	ON_COMMAND(ID_SPECTRUM_CROPFROM, OnSpectrumCropfrom)
	ON_COMMAND(ID_VIEWAS_OVERLAPPINGGRAPH, OnViewasOverlappinggraph)
	ON_UPDATE_COMMAND_UI(ID_VIEWAS_OVERLAPPINGGRAPH, OnUpdateViewasOverlappinggraph)
	ON_COMMAND(ID_PROCESSING_RESCALETOPOSITIVE, OnProcessingRescaletopositive)
	ON_COMMAND(ID_PROCESSING_REMOVENEGATIVE, OnProcessingRemovenegative)
	ON_COMMAND(ID_TOOLS_COMBINESPECTRA, OnToolsCombinespectra)
	ON_COMMAND(ID_TOOLS_SUBTRACTSPECTRA, OnToolsSubtractspectra)
	ON_COMMAND(ID_TOOLS_COMPARESPECTRA, OnToolsComparespectra)
	ON_COMMAND(ID_TOOLS_COMPAREALL, OnToolsCompareall)
	ON_COMMAND(ID_PROCESSING_SPECTRAALIGNMENTBYBESTFIT, OnProcessingSpectraalignmentbybestfit)
	ON_COMMAND(ID_SPECTRAALIGNMENT_COMBINEDMETHOD, OnSpectraalignmentCombinedmethod)
	ON_COMMAND(ID_SPECTRAALIGNMENT_CHOICE, OnSpectraalignmentChoice)
	ON_COMMAND(ID_VIEW_FASTDRAW, OnViewFastdraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FASTDRAW, OnUpdateViewFastdraw)
	ON_COMMAND(ID_TOOLS_GENERATECORRELELOGRAPH, OnToolsGeneratecorrelelograph)
	ON_COMMAND(ID_SPECTRAALIGNMENT_FFTMETHOD, OnSpectraalignmentFftmethod)
	ON_COMMAND(ID_EXPORT_PEAKS, OnExportPeaks)
	ON_COMMAND(ID_VIEW_BASELINE, OnViewBaseline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BASELINE, OnUpdateViewBaseline)
	ON_COMMAND(ID_SPECTRUM_CALCULATEPEAKAREAS, OnSpectrumCalculatepeakareas)
	ON_COMMAND(ID_EXPORT_PEAKAREA, OnExportPeakarea)
	ON_COMMAND(ID_PROCESSING_DENOISE, OnProcessingDenoise)
	ON_COMMAND(ID_TOOLS_EVALUATEDATASET, OnToolsEvaluatedataset)
	ON_COMMAND(ID_EXPORT_SPECTRAMODEL, OnExportSpectramodel)
END_MESSAGE_MAP()


// CSpecAlignDoc construction/destruction

CSpecAlignDoc::CSpecAlignDoc()
{
	// TODO: add one-time construction code here
	m_sizeDoc = CSize(2000, 2000000);
	showAvg = false;
	lineGraph = true;
	barGraph = false;
	stacked = false;
	overlap = false;
	showPeaks = false;
	peaksPicked = false;
	rel = false;
	fastDraw = false;
	viewBase = false;
	viewPModel = false;
	height = 1.5;
	cutoff = 0.5;
	window = 21;
	importType = 0;
	curpath = "";
	baseVal = 20;
	model = 1;
	denoise_sens = 0.5;
	denoise_recurs = 1;
	eval_segs = 1;
}

CSpecAlignDoc::~CSpecAlignDoc()
{
}

BOOL CSpecAlignDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CSpecAlignDoc serialization

void CSpecAlignDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CSpecAlignDoc diagnostics

#ifdef _DEBUG
void CSpecAlignDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpecAlignDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpecAlignDoc commands

void CSpecAlignDoc::OnImportFile()
{
	//if (importType == 2){
	//	AfxGetApp()->DoMessageBox(_T("Cannot import single files after importing folder. Try importing whole folders"),0,0);
	//	return;
	//}
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv|TAB delimited Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog fDlg(TRUE,"*.csv","*.*",OFN_HIDEREADONLY,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select single input file");
	if (fDlg.DoModal() == IDOK && fDlg.GetPathName() != ""){
		bool first;
		int orgsize = 0;
		CString tmp = fDlg.GetPathName();
		curpath = tmp;
		curpath.TrimRight(fDlg.GetFileName());

		first = dat.mz_val.empty();
		if (!first)
			orgsize = dat.train.size();
		AfxGetApp()->DoWaitCursor(1);
		LPTSTR lpsz = new TCHAR[tmp.GetLength()+1];
		_tcscpy(lpsz, tmp);
		int status = dat.readFileInput(0,lpsz,first, FALSE);
		if (status == 0){
			AfxGetApp()->DoMessageBox(_T("Error reading file"),0,0);
			return;
		}
		else if (status == -1){
			AfxGetApp()->DoMessageBox(_T("Range of new spectrum does not match loaded spectra"),0,0);
			return;
		}
		else if (status  == -2){
			AfxGetApp()->DoMessageBox(_T("File format not recognised - refer to helpfile for file format"),0,0);
			return;
		}
		CString s(fDlg.GetFileTitle());
		char t[256] = "";
		if (dat.train.size() == orgsize+1){
			if (dat.train.at(orgsize).name.GetLength() == 0)
				dat.train.at(orgsize).name = s;
		}
		else{
			for (unsigned int i = orgsize; i < dat.train.size(); ++i){
				if (dat.train.at(i).name.GetLength() == 0){
					s.SetString(fDlg.GetFileTitle());
					s.Append("_");
					s.Append(itoa((i-orgsize)+1,t,10));
					dat.train.at(i).name = s;
				}
				if (viewBase)
					dat.train.at(i).calBase(baseVal);
			}
		}
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	//	dat.sumSpec();
	//	showAvg = true;
		CRect size;
		pView->clearBackups();
		pView->GetWindowRect(&size);
		pView->drawSpec(true, fastDraw);
		pView->viewAllSpec(fastDraw);
		pView->updatePane(size.Width()-20);
	//	UpdateAllViews(NULL);
		if (dat.mz_val.size() < 80)
			barGraph = true;
		if (barGraph){
			barGraph=false;
			lineGraph = true;
			OnViewasBargraph();
		}
		AfxGetApp()->DoWaitCursor(-1);
		importType = 1;
	}
}

void CSpecAlignDoc::OnEditUndo()
{
	if (dat.train.empty() || dat.last.empty())
		return;
	AfxGetApp()->DoWaitCursor(1);
	int type = dat.copyFromLast();
	if (type == 1)
		rel = false;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (type == 3 || type == 6){
		pView->clearBackups();
	}
	if (type == 10 || type == 11){
	}
	else if (type == 2 || type == 7)
		pView->drawSpec(FALSE, fastDraw);
	else{
		bool tvAll = pView->vAll;
		pView->vAll = false;
		pView->drawSpec(TRUE, fastDraw);
		pView->vAll = tvAll;
	}
	if (barGraph){
		if (!dat.avg.input.empty()){
			pView->avg.spec.SetElementLinetype(9);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].spec.SetElementLinetype(9);
		}
	}
	pView->viewAllSpec(fastDraw);
	if (dat.train.at(0).peaks.size() != 0 && type != 10 && type != 11){
		//OnSpectrumRemovepeaks();
		dat.pickPeaks(cutoff,window,height, true);
		peaksPicked = true;
		//pView->drawAnno();
	}
	pView->drawAnno();
	UpdateAllViews(NULL);	
	AfxGetApp()->DoWaitCursor(-1);
}

void CSpecAlignDoc::OnEditRevert()
{
	if (dat.train.empty())
		return;
	AfxGetApp()->DoWaitCursor(1);
	OnSpectrumRemovepeaks();
	dat.copyFromBackup();
	if (dat.avg.input.size() != 0 && dat.avg.input.size()!= dat.mz_val.size()){
		dat.sumSpec();
	}
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	pView->clearBackups();
	bool vState = pView->vAll;
	pView->vAll = false;
	pView->drawSpec(true, fastDraw);
	if (barGraph){
		if (!dat.avg.input.empty()){
			pView->avg.spec.SetElementLinetype(9);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].spec.SetElementLinetype(9);
		}
	}
	pView->vAll = vState;
	pView->viewAllSpec(fastDraw);
	//OnSpectrumRemovepeaks();
	//dat.pickPeaks(cutoff,window,peakIter);
	//peaksPicked = true;
	pView->drawAnno();
	peaksPicked = false;
	showPeaks = false;
	rel = false;
	//UpdateAllViews(NULL);	
	AfxGetApp()->DoWaitCursor(-1);
}

void CSpecAlignDoc::OnProcessingNormalizetic()
{
	if (dat.train.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	dlgCrop cDlg;
	cDlg.lo = pView->spectra[0].m_minx;
	cDlg.up = pView->spectra[0].m_maxx;
	if (cDlg.DoModal() == IDOK){
		if (cDlg.lo> cDlg.up){
			double tmp = cDlg.lo;
			cDlg.lo=cDlg.up;
			cDlg.up= tmp;
		}
		if (cDlg.lo< dat.mz_val.front() || cDlg.up> dat.mz_val.back()){
			AfxGetApp()->DoMessageBox(_T("Range specified exceeds limits of dataset"),0,0);
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
		bool oP = dat.train.at(0).peaks.size() != 0;
		dat.normalizeSpec(dat.getXIndex(cDlg.lo),dat.getXIndex(cDlg.up));

		if (!dat.avg.input.empty()){
			OnSpectrumGenerateaveragespectrum();
		}
		pView->clearBackups();
		bool vState = pView->vAll;
		pView->vAll = false;
		pView->drawSpec(true, fastDraw);
		if (!viewBase)
			pView->removeBase();
		pView->vAll = vState;
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnProcessingRelativeintensities()
{
	if (dat.train.empty())
		return;
	rel = true;
	AfxGetApp()->DoWaitCursor(1);
	bool oP = dat.train.at(0).peaks.size() != 0;
	//if (peaksPicked)
	//	OnSpectrumRemovepeaks();
	dat.convertToRelative();
	//if (oP){
	//	dat.pickPeaks(cutoff,window,peakIter, false);
	//	peaksPicked = true;
	//}
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	//if (peaksPicked){
	//	OnSpectrumRemovepeaks();
	//	dat.pickPeaks(cutoff,window,peakIter);
	//}
	pView->clearBackups();
	pView->drawSpec(true, fastDraw);
	if (!viewBase)
		pView->removeBase();
	pView->viewAllSpec(fastDraw);
	pView->drawAnno();
	UpdateAllViews(NULL);
	AfxGetApp()->DoWaitCursor(-1);
}

void CSpecAlignDoc::OnViewAveragespec()
{
	if (dat.train.empty() || dat.avg.input.empty())
		return;
	if (showAvg)
		showAvg = false;
	else
		showAvg = true;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	CRect size;
	pView->GetWindowRect(&size);
	pView->updatePane(size.Width()-20);
	UpdateAllViews(NULL);
	pView->viewAllSpec(fastDraw);
}

void CSpecAlignDoc::OnUpdateViewAveragespec(CCmdUI *pCmdUI)
{
	if(showAvg)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnGraphTrack()
{

	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Track){
		pView->m_Track = false;
		if (!dat.avg.input.empty()){
			pView->avg.spec.DeleteCursor(0);
			pView->avg.spec.SetTrackMode(0);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].spec.DeleteCursor(0);
			pView->spectra[i].spec.SetTrackMode(0);
		}
		if (pView->allSpec.created){
			pView->allSpec.spec.DeleteCursor(0);
			pView->allSpec.spec.SetTrackMode(0);
		}
	}
	else{
		pView->m_Track = true;
		if (!dat.avg.input.empty()){
			pView->avg.spec.AddCursor();
		//	pView->avg.spec.SetCursorVisible(true);
			pView->avg.spec.SetTrackMode(1);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].spec.AddCursor();
		//	pView->spectra[i].spec.SetCursorVisible(true);
			pView->spectra[i].spec.SetTrackMode(1);
		}
		if (pView->allSpec.created){
			pView->allSpec.spec.AddCursor();
			pView->allSpec.spec.SetTrackMode(1);
		}
	}
}

void CSpecAlignDoc::OnUpdateGraphTrack(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Track)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnGraphZoom()
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Zoom){		
		pView->m_Zoom = false;
	}
	else{
		pView->m_Zoom = true;
	}
}

void CSpecAlignDoc::OnUpdateGraphZoom(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Zoom)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

//void CSpecAlignDoc::OnGraphPan()
//{
//	POSITION pos = GetFirstViewPosition();
//	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
//	if (pView->m_TrackMode != 2){
//		pView->m_TrackMode = 2;
//	}
//}
//
//void CSpecAlignDoc::OnUpdateGraphPan(CCmdUI *pCmdUI)
//{
//	POSITION pos = GetFirstViewPosition();
//	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
//	if (pView->m_TrackMode == 2){
//		pCmdUI->SetCheck(TRUE);
//		CMenu *m = pCmdUI->m_pMenu;
//		m->CheckMenuItem(m->GetMenuItemID(0),0);
//		m->CheckMenuItem(m->GetMenuItemID(1),0);
//	}
//}

void CSpecAlignDoc::OnEditUndoZoom()
{
	if (dat.train.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!dat.train.empty()){   
		if (!dat.avg.input.empty() && showAvg){
			pView->avg.undoRange();
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].undoRange();
		}
		pView->allSpec.undoRange();
	}	
}

void CSpecAlignDoc::OnEditRevertallzoom()
{
	if (dat.train.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!dat.train.empty()){   
		if (!dat.avg.input.empty()){
			pView->avg.revertRange(rel);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].revertRange(rel);
		}
		if (pView->allSpec.created)
			pView->allSpec.revertRange(false);
	}	
}

void CSpecAlignDoc::OnViewasLinegraph()
{
	if (dat.train.empty())
		return;
	if (barGraph || stacked || overlap){

		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		pView->vAll = false;
		if (stacked||overlap){
			pView->drawSpec(FALSE, fastDraw);
			showPeaks = false;
			pView->drawAnno();
		}
		if (!dat.avg.input.empty()){
			//if (!dat.avg.base.empty()){
			//	pView->avg.showBase();
				pView->avg.spec.SetElement(0);
			//}
			pView->avg.spec.SetElementLinetype(0);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			//if (!dat.train.at(i).base.empty()){
			//	pView->spectra[i].showBase();
				pView->spectra[i].spec.SetElement(0);
			//}
			pView->spectra[i].spec.SetElementLinetype(0);
		}
		lineGraph = true;
		barGraph = false;
		stacked = false;
		overlap = false;
	}
}

void CSpecAlignDoc::OnUpdateViewasLinegraph(CCmdUI *pCmdUI)
{
	if (lineGraph){
		pCmdUI->SetCheck(TRUE);
		CMenu *m = pCmdUI->m_pMenu;
		if (m != NULL){
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex+1),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex+2),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex+3),0);
		}
	}
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnViewasBargraph()
{
	if (dat.train.empty())
		return;
	if (lineGraph||stacked||overlap){
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		pView->vAll = false;
		if (stacked||overlap){
			pView->drawSpec(FALSE, fastDraw);
			showPeaks = false;
			pView->drawAnno();
		}
		if (!dat.avg.input.empty()){
			//if (!dat.avg.base.empty()){
			//	pView->avg.removeBase();
			//	pView->avg.spec.SetElement(0);
			//}
			pView->avg.spec.SetElement(0);
			pView->avg.spec.SetElementLinetype(9);
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			//if (!dat.train.at(i).base.empty()){
			//	pView->spectra[i].removeBase();
			//	pView->spectra[i].spec.SetElement(0);
			//}
			pView->spectra[i].spec.SetElement(0);
			pView->spectra[i].spec.SetElementLinetype(9);
		}
		barGraph = true;
		lineGraph = false;
		stacked = false;
		overlap = false;
	}
}

void CSpecAlignDoc::OnUpdateViewasBargraph(CCmdUI *pCmdUI)
{
	if (barGraph){
		pCmdUI->SetCheck(TRUE);
		CMenu *m = pCmdUI->m_pMenu;
		if (m != NULL){
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex-1),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex+1),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex+2),0);
		}
	}
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnViewasStackedgraph()
{
	if (dat.train.empty())
		return;
	if (lineGraph||barGraph||overlap){
		barGraph = false;
		lineGraph = false;
		stacked =true;
		overlap = false;
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		pView->vAll = true;
		pView->viewAllSpec(fastDraw);
	}
}

void CSpecAlignDoc::OnUpdateViewasStackedgraph(CCmdUI *pCmdUI)
{
	if (stacked){
		pCmdUI->SetCheck(TRUE);
		CMenu *m = pCmdUI->m_pMenu;
		if (m != NULL){
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex-1),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex-2),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex+1),0);
		}
	}
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnViewasOverlappinggraph()
{
	if (dat.train.empty())
		return;
	if (lineGraph||barGraph||stacked){
		barGraph = false;
		lineGraph = false;
		stacked = false;
		overlap = true;
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		pView->vAll = true;
		pView->viewAllSpec(fastDraw);
	}
}

void CSpecAlignDoc::OnUpdateViewasOverlappinggraph(CCmdUI *pCmdUI)
{
	if (overlap){
		pCmdUI->SetCheck(TRUE);
		CMenu *m = pCmdUI->m_pMenu;
		if (m != NULL){
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex-1),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex-2),0);
			m->CheckMenuItem(m->GetMenuItemID(pCmdUI->m_nIndex-3),0);
		}
	}
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnSpectrumPickpeaks()
{
	if (dat.train.empty())
		return;
	peakDlg	pDlg;
	pDlg.m_cutoff = cutoff;
	pDlg.m_win = window;
	pDlg.m_height = height;
	if (pDlg.DoModal() == IDOK){
		if (pDlg.m_win > dat.mz_val.size()){
			AfxGetApp()->DoMessageBox(_T("Window too large"),0,0);
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
	//	OnSpectrumRemovepeaks();
		dat.pickPeaks(pDlg.m_cutoff,pDlg.m_win, pDlg.m_height, true);
		height = pDlg.m_height;
		cutoff = pDlg.m_cutoff;
		window = pDlg.m_win;
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		if (showPeaks)
			pView->drawPeaks();
		pView->drawAnno();
		AfxGetApp()->DoWaitCursor(-1);
		peaksPicked = true;
	}
	
}

void CSpecAlignDoc::OnSpectrumRemovepeaks()
{
	if (dat.train.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!dat.train.empty() && !dat.train.at(0).peaks.empty()){   
		if (!dat.avg.input.empty() && showAvg){
			pView->avg.spec.DeleteAnnotation(0);
			pView->avg.removeAllModels();
		}
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			pView->spectra[i].spec.DeleteAnnotation(0);
			pView->spectra[i].removeAllModels();
		}
	}
	if (showPeaks){
		pView->drawSpec(FALSE, fastDraw);
		showPeaks = false;
	}
	dat.clearPeaks(true);
	peaksPicked = false;

}

void CSpecAlignDoc::OnProcessingAlignpeaks()
{
	if (dat.mz_val.empty())
		return;
	if (dat.avg.input.empty()){
			AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
			return;
	}
	bool cont = true;
	//if (dat.avg.peaks.size() > 100){
	//	warnDlg wDlg;
	//	if (wDlg.DoModal() != IDOK){
	//		cont = false;
	//	}
	//}
	if (cont){
		peakMarDlg	pDlg;
		pDlg.start = dat.mz_val.front();
		pDlg.end = dat.mz_val.back();
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		pDlg.lower = pView->spectra[0].m_minx;
		pDlg.upper = pView->spectra[0].m_maxx;
		if (dat.avg.input.size() == 0)
			OnSpectrumGenerateaveragespectrum();
		if (pDlg.DoModal() == IDOK){
			if (pDlg.lower> pDlg.upper){
				double tmp = pDlg.lower;
				pDlg.lower=pDlg.upper;
				pDlg.upper= tmp;
			}
			if (pDlg.lower< pDlg.start || pDlg.upper> pDlg.end){
				AfxGetApp()->DoMessageBox(_T("Range specified exceeds limits of dataset"),0,0);
				return;
			}
			AfxGetApp()->DoWaitCursor(1);
			dat.alignPeaks(pDlg.margin, cutoff, window, height, pDlg.lower, pDlg.upper);
			dat.pickPeaks(cutoff, window,height,true);
			if (showPeaks){
				pView->drawPeaks();
			}
			else
				pView->drawSpec(FALSE, fastDraw);
			if (barGraph){
				if (!dat.avg.input.empty()){
					pView->avg.spec.SetElementLinetype(9);
				}
				for (unsigned int i = 0; i < dat.train.size(); ++i){
					pView->spectra[i].spec.SetElementLinetype(9);
				}
			}
				
			pView->viewAllSpec(fastDraw);
			pView->drawAnno();
			AfxGetApp()->DoWaitCursor(-1);
			peaksPicked= true;
		}
	}
}

void CSpecAlignDoc::OnSpectrumPlotpeaks()
{
	if (dat.train.empty())
		return;
	if (dat.train.at(0).peaks.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (showPeaks){
		showPeaks = false;
		pView->drawSpec(FALSE, fastDraw);
	}
	else{
		showPeaks = true;
		pView->drawPeaks();
	}
	pView->viewAllSpec(fastDraw);
}

void CSpecAlignDoc::OnUpdateSpectrumPlotpeaks(CCmdUI *pCmdUI)
{
	if (showPeaks)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CSpecAlignDoc::OnImportFolder()
{
	int orgsize = dat.train.size();
	CFolderDialog fDlg(_T("Select Folder for import..."),"",NULL,BIF_NEWDIALOGSTYLE|BIF_NONEWFOLDERBUTTON);
	if (fDlg.DoModal() == IDOK && fDlg.GetFolderPath() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFolderPath();
		curpath = fDlg.GetFolderPath();
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		int status = dat.readFolderInput(lpsz);
		if (status == 0){
			AfxGetApp()->DoMessageBox(_T("Error reading file"),0,0);
			return;
		}
		else if (status == -1){
			AfxGetApp()->DoMessageBox(_T("Folder does not contain spectrum files"),0,0);
			return;
		}
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	//	dat.sumSpec();
	//	showAvg = true;
		CRect size;
		pView->clearBackups();
		pView->GetWindowRect(&size);
		pView->drawSpec(true, fastDraw);
		pView->viewAllSpec(fastDraw);
		pView->updatePane(size.Width()-20);
		//char text[256] ="Average";
		//char buf[256]="";
		//pView->avg.label.SetWindowText(text);
		//for (unsigned int i = orgsize; i < dat.train.size(); ++i){
		//	itoa(i,text,10);
		//	pView->spectra[i].label.SetWindowText(text);
		//}
		if (dat.mz_val.size() < 80)
			barGraph = true;
		if (barGraph){
			barGraph=false;
			lineGraph = true;
			OnViewasBargraph();
		}
		//UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
		importType = 2;
	}
}

void CSpecAlignDoc::OnSpectrumCrop()
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!pView->m_Crop)
		pView->m_Crop = true;
	else
		pView->m_Crop = false;
}

void CSpecAlignDoc::OnExportFile()
{
	if (dat.train.empty())
		return;
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv";
	CFileDialog fDlg(FALSE,"*.csv","*.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select filename");
	if (fDlg.DoModal() == IDOK && fDlg.GetFileName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFileName();
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		dat.exportSingleFile(lpsz, FALSE);
		//dat.exportSingleFileRot(lpsz, FALSE);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnExportMultifile()
{
	if (dat.train.empty())
		return;
	LPTSTR clpsz = new TCHAR[curpath.GetLength()+1];
	_tcscpy(clpsz, curpath);
	CFolderDialog fDlg(_T("Select Folder for export..."),clpsz,NULL,BIF_NEWDIALOGSTYLE|BIF_SHAREABLE);
	if (fDlg.DoModal() == IDOK && fDlg.GetFolderPath() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFolderPath();
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		dat.exportMultiFile(lpsz);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnSpectrumRemovespectrum()
{
	if (dat.train.empty())
		return;
	selectSpecDlg rDlg;
	if (rDlg.DoModal() == IDOK){
		CString r = rDlg.rspec;
		CString tmp;
		int curPos = 0;
		vector <int> rVals;
		tmp = r.Tokenize(",",curPos);
		while (tmp != "")
		{
			if (tmp.Find('-',0) != -1){
				int nPos = 0;
				CString start = tmp.Tokenize("-",nPos);
				CString end = tmp.Tokenize("-",nPos);
				LPTSTR lpsz = new TCHAR[start.GetLength()+1];
				_tcscpy(lpsz, start);
				int st = atoi(lpsz);
				lpsz = new TCHAR[end.GetLength()+1];
				_tcscpy(lpsz, end);
				int ed = atoi(lpsz);
				for (int i = st; i <= ed; ++i){
					rVals.push_back(i);
				}
			}
			else{
				LPTSTR lpsz = new TCHAR[tmp.GetLength()+1];
				_tcscpy(lpsz, tmp);
				rVals.push_back(atoi(lpsz));
			}
			tmp = r.Tokenize(",",curPos);
		}
		for (unsigned int i = 0; i < rVals.size(); ++i){
			if (rVals.at(i)-1 < 0 || rVals.at(i)-1 >= dat.train.size()){
				AfxGetApp()->DoMessageBox(_T("Specified spectrum does not exist"),0,0);
				return;
			}
		}
		AfxGetApp()->DoWaitCursor(1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		for (unsigned int i = 0; i < rVals.size(); ++i){
			dat.removeSpec(rVals.at(i)-1);
			pView->rmSpec(rVals.at(i)-1);
			for (unsigned int j = i+1; j < rVals.size(); ++j){
				if (rVals.at(j) > rVals.at(i))
					rVals.at(j)--;
			}
		}
		if (dat.train.size() != 0){
			//dat.sumSpec();
			this->OnEditRevertallzoom();
			if (!showPeaks)
				pView->drawSpec(FALSE, fastDraw);
			else
				pView->drawPeaks();
			if (dat.train.at(0).peaks.size() != 0){
				dat.pickPeaks(cutoff,window,height, true);
				////OnSpectrumRemovepeaks();
				////dat.pickPeaks(cutoff,window,peakIter);
				pView->drawAnno();
			}
			pView->viewAllSpec(fastDraw);
		}
		else{
			pView->avg.ShowWindow(SW_HIDE);
		}
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
}


void CSpecAlignDoc::OnFileOpen()
{
	if (!dat.train.empty()){
			AfxGetApp()->DoMessageBox(_T("File already opened. Create new document first."),0,0);
			return;
	}
	static char BASED_CODE szFilter[] = "SPA Files (*.spa)|*.spa";
	CFileDialog fDlg(TRUE,"*.spa","*.spa",OFN_HIDEREADONLY,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select File to Open");
	if (fDlg.DoModal() == IDOK && fDlg.GetPathName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFileName();
		CString tmp = decompress(path);
		LPTSTR lpsz = new TCHAR[tmp.GetLength()+1];
		_tcscpy(lpsz, tmp);
		bool first = true;
		int status = dat.readData(lpsz);
		if (status != 1){
			AfxGetApp()->DoMessageBox(_T("Error reading file"),0,0);
			return;
		}
		dat.readFileInput(0,lpsz,first,TRUE);
		remove(lpsz);
		if (!dat.train.at(0).base.empty()){
			viewBase = true;
			CWnd* pMain = AfxGetMainWnd();
			if (pMain != NULL){
				CMenu* pMenu = pMain->GetMenu();
				if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
					pMenu->CheckMenuItem(ID_VIEW_BASELINE,1);
			}
		}
		if (!dat.train.at(0).peaks.empty()){
			peaksPicked = true;
			if (dat.train.at(0).peaks.at(0).a != 0)
				viewPModel = true;
		}
		if (!dat.avg.input.empty()){
			showAvg = true;
			CWnd* pMain = AfxGetMainWnd();
			if (pMain != NULL){
				CMenu* pMenu = pMain->GetMenu();
				if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
					pMenu->CheckMenuItem(ID_VIEW_AVERAGESPEC,1);
			}
		}
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		//if (showAvg){
		//	pView->avg.d = &(dat.avg);
		//	pView->avg.ID = -1;
		//}
		CRect size;
		pView->GetWindowRect(&size);
		pView->drawSpec(true, fastDraw);
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		pView->updatePane(size.Width()-20);
	//	UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
		curSpath = path;
	}
}

void CSpecAlignDoc::OnFileSaveAs()
{
	if (dat.train.empty())
		return;
	static char BASED_CODE szFilter[] = "SPA Files (*.spa)|*.spa";
	CFileDialog fDlg(FALSE,"*.spa","*.spa",OFN_HIDEREADONLY,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Save As");
	if (fDlg.DoModal() == IDOK && fDlg.GetPathName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFileName();
		path.Append("z");
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		int status = dat.saveData(lpsz);
		if (status != 1){
			AfxGetApp()->DoMessageBox(_T("Error writing to file"),0,0);
			return;
		}
		dat.exportSingleFile(lpsz,TRUE);
		compress(path);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

BOOL CSpecAlignDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	if (lpszPathName != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = lpszPathName;
		CString tmp = decompress(path);
		LPTSTR lpsz = new TCHAR[tmp.GetLength()+1];
		_tcscpy(lpsz, tmp);
		bool first = true;
		int status = dat.readData(lpsz);
		if (status != 1){
			AfxGetApp()->DoMessageBox(_T("Error reading file"),0,0);
			return FALSE;
		}
		dat.readFileInput(0,lpsz,first,TRUE);
		remove(lpsz);
		if (!dat.train.at(0).base.empty()){
			viewBase = true;
			CWnd* pMain = AfxGetMainWnd();
			if (pMain != NULL){
				CMenu* pMenu = pMain->GetMenu();
				if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
					pMenu->CheckMenuItem(ID_VIEW_BASELINE,1);
			}
		}
		if (!dat.train.at(0).peaks.empty()){
			peaksPicked = true;
			if (dat.train.at(0).peaks.at(0).a != 0)
				viewPModel = true;
		}
		if (!dat.avg.input.empty()){
			dat.avg.input.clear();
			dat.avg.peaks.clear();
			//showAvg = true;
			//CWnd* pMain = AfxGetMainWnd();
			//if (pMain != NULL){
			//	CMenu* pMenu = pMain->GetMenu();
			//	if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
			//		pMenu->CheckMenuItem(ID_VIEW_AVERAGESPEC,1);
			//}
		}
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		CRect size;
		pView->drawSpec(true, fastDraw);
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		pView->GetWindowRect(&size);
	//	UpdateAllViews(NULL);
		pView->updatePane(size.Width()-20);
		AfxGetApp()->DoWaitCursor(-1);
		curSpath = path;
	}

	return TRUE;
}

void CSpecAlignDoc::OnFileSave()
{
	if (curSpath.GetLength() == 0)
		return;
	AfxGetApp()->DoWaitCursor(1);
	CString path = curSpath;
	path.Append("z");
	LPTSTR lpsz = new TCHAR[path.GetLength()+1];
	_tcscpy(lpsz, path);
	int status = dat.saveData(lpsz);
	if (status != 1){
		AfxGetApp()->DoMessageBox(_T("Error writing to file"),0,0);
		return;
	}
	dat.exportSingleFile(lpsz,TRUE);
	compress(path);
	AfxGetApp()->DoWaitCursor(-1);
}

CString CSpecAlignDoc::decompress(CString src)
{
	char * tempfile = NULL;
	tempfile = _tempnam("c:\\tmp", "stq");
	CString temp = tempfile;
	LPTSTR lpsz = new TCHAR[src.GetLength()+1];
	_tcscpy(lpsz,src);
	HZIP hz = OpenZip((void *)lpsz,0, ZIP_FILENAME);
	UnzipItem(hz,0, tempfile,0,ZIP_FILENAME);
	CloseZip(hz);
	if(tempfile)
		free(tempfile);
	return temp;
}

void CSpecAlignDoc::compress(CString src)
{
	CString des = src;
	des.TrimRight("z");
	LPTSTR dlpsz = new TCHAR[des.GetLength()+1];
	_tcscpy(dlpsz,des);
	LPTSTR slpsz = new TCHAR[src.GetLength()+1];
	_tcscpy(slpsz,src);
	HZIP hz = CreateZip((void *)dlpsz,0, ZIP_FILENAME);
	ZipAdd(hz,dlpsz,slpsz,0,ZIP_FILENAME);
	CloseZip(hz);
	remove(slpsz);
}

void CSpecAlignDoc::OnUpdateSpectrumCrop(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Crop )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}


void CSpecAlignDoc::OnProcessingbins()
{
	if (dat.train.empty())
			return;
	binDlg bDlg;
	if (bDlg.DoModal() == IDOK){
		if (bDlg.binsize <= 0.0001){
			AfxGetApp()->DoMessageBox(_T("Bin size too small"),0,0);
			return;
		}
		if (bDlg.binsize > dat.mz_val.back()){
			AfxGetApp()->DoMessageBox(_T("Bin size too large"),0,0);
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
		vector <double> oldppos;
		for (unsigned int i = 0; i < dat.train.at(0).peaks.size(); ++i){
			oldppos.push_back(dat.train.at(0).peaks.at(i).mz);
		}
		dat.binSpectra(bDlg.binsize);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		dat.clearPeaks(false);
		for (unsigned int i = 0; i < oldppos.size(); ++i){
			dat.addPeak(oldppos.at(i), false);
		}
		if (viewBase){
			pView->removeBase();
			dat.calBase(baseVal);
		}
		pView->clearBackups();
		pView->drawSpec(true, fastDraw);
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		UpdateAllViews(NULL);
		if (barGraph){
			barGraph=false;
			lineGraph = true;
			OnViewasBargraph();
		}
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnProcessingSmooth()
{
	if (dat.train.empty())
			return;
	smoothDlg sDlg;
	if (sDlg.DoModal() == IDOK){
		AfxGetApp()->DoWaitCursor(1);
		vector <double> oldppos;
		for (unsigned int i = 0; i < dat.train.at(0).peaks.size(); ++i){
			oldppos.push_back(dat.train.at(0).peaks.at(i).mz);
		}
		dat.smoothSpec(sDlg.fsize);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		dat.clearPeaks(false);
		for (unsigned int i = 0; i < oldppos.size(); ++i){
			dat.addPeak(oldppos.at(i), false);
		}
		pView->drawSpec(FALSE, fastDraw);
		if (!viewBase)
			pView->removeBase();
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnSpectrumGenerateaveragespectrum()
{
	if (dat.train.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	bool newAvg = !dat.avg.input.empty();
	dat.sumSpec();
	if (viewBase)
		dat.avg.calBase(baseVal);
	if (!pView->avg.created)
		pView->createAvg();
	pView->avg.d = &(dat.avg);
	pView->avg.ID = -1;
	pView->avg.drawSpec(TRUE, fastDraw);
	if(pView->vAll)
		pView->avg.ShowWindow(SW_HIDE);
	//pView->viewAllSpec();
	int fxmin = dat.getXIndex(pView->spectra[0].m_bminx);
	int fxmax = dat.getXIndex(pView->spectra[0].m_bmaxx);
	int xmin = dat.getXIndex(pView->spectra[0].m_minx);
	int xmax = dat.getXIndex(pView->spectra[0].m_maxx);
	pView->avg.updateCropRange();
	pView->avg.setRange(pView->spectra[0].m_bminx,pView->spectra[0].m_bmaxx,dat.avg.getMinY(fxmin,fxmax)*1.15,dat.avg.getMaxY(fxmin,fxmax)*1.15);
	pView->avg.setRange(pView->spectra[0].m_minx,pView->spectra[0].m_maxx,dat.avg.getMinY(xmin,xmax)*1.15,dat.avg.getMaxY(xmin,xmax)*1.15);
	if (dat.train.at(0).peaks.size() != 0){
		dat.avg.peaksAt(dat.train.at(0).peaks);
		for (unsigned int i = 0; i < dat.train.at(0).manualPeaks.size(); ++i){
			dat.avg.manualPeaks.push_back(dat.train.at(0).manualPeaks.at(i));
		}
		pView->avg.drawAnno();
		if (viewPModel && !dat.avg.peaks.at(0).modelled){
			dat.avg.modelPeaks(model,INT_MAX,INT_MIN,true);
			pView->avg.showModels();
		}
	}
	showAvg = true;
	if (showPeaks){
		pView->avg.plotP = true;
		pView->avg.plotPeaks();
	}
	CRect size;
	pView->GetWindowRect(&size);
	pView->updatePane(size.Width()-20);
	UpdateAllViews(NULL);
	if (barGraph){
		barGraph=false;
		lineGraph = true;
		OnViewasBargraph();
	}
	pView->viewAllSpec(fastDraw);
}

void CSpecAlignDoc::OnExportAveragespectrum()
{
	if (dat.train.empty())
		return;
	if (dat.avg.input.empty()){
		AfxGetApp()->DoMessageBox(_T("No average spectrum to export"),0,0);
		return;
	}
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv";
	CFileDialog fDlg(FALSE,"*.csv","*.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select average spectrum output filename");
	if (fDlg.DoModal() == IDOK && fDlg.GetFileName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFileName();
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		int status = dat.exportAvg(lpsz, FALSE);
		if (status == 0){
			AfxGetApp()->DoMessageBox(_T("Average spectrum exporting failed"),0,0);
			AfxGetApp()->DoWaitCursor(-1);
			return;
		}

		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnImportAveragespectrum()
{
	if (dat.train.empty()){
		AfxGetApp()->DoMessageBox(_T("Please load data spectrum first."),0,0);
		return;
	}
	if (!dat.avg.input.empty()){
			AfxGetApp()->DoMessageBox(_T("Exisiting average spectrum may be replaced."),0,0);
	}
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv||All Files (*.*)|*.*||";
	CFileDialog fDlg(TRUE,"*.csv","*.*",OFN_HIDEREADONLY,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select average spectrum input file");
	if (fDlg.DoModal() == IDOK && fDlg.GetPathName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		LPTSTR lpsz = new TCHAR[fDlg.GetPathName().GetLength()+1];
		_tcscpy(lpsz, fDlg.GetPathName());
		int status = dat.readAvg(lpsz, FALSE);
		if (status == 0){
			AfxGetApp()->DoMessageBox(_T("Error reading file"),0,0);
			return;
		}
		else if (status == -1){
			AfxGetApp()->DoMessageBox(_T("Range of new spectrum does not match existing spectra"),0,0);
			return;
		}
		else if (status  == -2){
			AfxGetApp()->DoMessageBox(_T("File format not recognised - refer to helpfile for file format"),0,0);
			return;
		}
		if (viewBase)
			dat.avg.calBase(baseVal);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		if (!pView->avg.created)
			pView->createAvg();
		pView->avg.d = &(dat.avg);
		showAvg = true;
		pView->avg.ID = -1;
		pView->avg.drawSpec(TRUE, fastDraw);
		if(pView->vAll)
			pView->avg.ShowWindow(SW_HIDE);
	//	pView->viewAllSpec();
	int fxmin = dat.getXIndex(pView->spectra[0].m_bminx);
	int fxmax = dat.getXIndex(pView->spectra[0].m_bmaxx);
	int xmin = dat.getXIndex(pView->spectra[0].m_minx);
	int xmax = dat.getXIndex(pView->spectra[0].m_maxx);
	pView->avg.updateCropRange();
	pView->avg.setRange(pView->spectra[0].m_bminx,pView->spectra[0].m_bmaxx,dat.avg.getMinY(fxmin,fxmax)*1.15,dat.avg.getMaxY(fxmin,fxmax)*1.15);
	pView->avg.setRange(pView->spectra[0].m_minx,pView->spectra[0].m_maxx,dat.avg.getMinY(xmin,xmax)*1.15,dat.avg.getMaxY(xmin,xmax)*1.15);
	if (dat.train.at(0).peaks.size() != 0){
		dat.avg.peaksAt(dat.train.at(0).peaks);
		for (unsigned int i = 0; i < dat.train.at(0).manualPeaks.size(); ++i){
			dat.avg.manualPeaks.push_back(dat.train.at(0).manualPeaks.at(i));
		}
		if (viewPModel){
			dat.avg.modelPeaks(model,INT_MAX,INT_MIN,true);
			pView->avg.showModels();
		}
		pView->avg.drawAnno();
	}
		if (showPeaks){
			pView->avg.plotP = true;
			pView->avg.plotPeaks();
		}

		CRect size;
		pView->GetWindowRect(&size);
		pView->updatePane(size.Width()-20);
		UpdateAllViews(NULL);
		pView->viewAllSpec(fastDraw);
		if (barGraph){
			barGraph=false;
			lineGraph = true;
			OnViewasBargraph();
		}
	}
}

void CSpecAlignDoc::OnFilePrint()
{
	if (dat.train.empty())
	return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->allSpec.created)
		pView->allSpec.spec.PrintGraph();
	else{
		pView->vAll = true;
		pView->viewAllSpec(fastDraw);
		pView->allSpec.spec.PrintGraph();
		pView->vAll = false;
		pView->setUnStackVisible();
	}
}

void CSpecAlignDoc::OnEditCopy()
{
	if (dat.train.empty())
	return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->allSpec.created)
		pView->allSpec.spec.CopyToClipboard();
	else{
		pView->vAll = true;
		pView->viewAllSpec(fastDraw);
		pView->allSpec.spec.CopyToClipboard();
		pView->vAll = false;
		pView->setUnStackVisible();
	}
}

void CSpecAlignDoc::OnSpectrumMergewithaveragespectrum()
{
	if (dat.train.empty())
		return;
	dat.mergeAvg();
	dat.avg.name = "Average";
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!pView->avg.created)
		pView->createAvg();
	if (viewBase)
		dat.avg.calBase(baseVal);
	pView->avg.d = &(dat.avg);
	showAvg = true;
	pView->avg.ID = -1;
	pView->avg.drawSpec(TRUE, fastDraw);
	if(pView->vAll)
		pView->avg.ShowWindow(SW_HIDE);
//	pView->viewAllSpec();
	int fxmin = dat.getXIndex(pView->spectra[0].m_bminx);
	int fxmax = dat.getXIndex(pView->spectra[0].m_bmaxx);
	int xmin = dat.getXIndex(pView->spectra[0].m_minx);
	int xmax = dat.getXIndex(pView->spectra[0].m_maxx);
	pView->avg.updateCropRange();
	pView->avg.setRange(pView->spectra[0].m_bminx,pView->spectra[0].m_bmaxx,dat.avg.getMinY(fxmin,fxmax)*1.15,dat.avg.getMaxY(fxmin,fxmax)*1.15);
	pView->avg.setRange(pView->spectra[0].m_minx,pView->spectra[0].m_maxx,dat.avg.getMinY(xmin,xmax)*1.15,dat.avg.getMaxY(xmin,xmax)*1.15);
	if (dat.train.at(0).peaks.size() != 0){
		dat.avg.peaksAt(dat.train.at(0).peaks);
		for (unsigned int i = 0; i < dat.train.at(0).manualPeaks.size(); ++i){
			dat.avg.manualPeaks.push_back(dat.train.at(0).manualPeaks.at(i));
		}
		if (viewPModel){
			dat.avg.modelPeaks(model,INT_MAX,INT_MIN,true);
			pView->avg.showModels();
		}
		pView->avg.drawAnno();
	}
	if (showPeaks){
		pView->avg.plotP = true;
		pView->avg.plotPeaks();
	}
	CRect size;
	pView->GetWindowRect(&size);
	pView->updatePane(size.Width()-20);
	UpdateAllViews(NULL);
	if (barGraph){
		barGraph=false;
		lineGraph = true;
		OnViewasBargraph();
	}
}

void CSpecAlignDoc::OnProcessingScale()
{
	if (dat.train.empty())
			return;
	scaleDlg sDlg;
	if (sDlg.DoModal() == IDOK){
		if (sDlg.fact == 0 || sDlg.fact == 1){
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
		dat.scale(sDlg.fact);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		//if (dat.train.at(0).peaks.size() != 0){
		//	//OnSpectrumRemovepeaks();
		//	dat.pickPeaks(cutoff,window,peakIter, false);
		//	peaksPicked = true;
		//}
		pView->drawSpec(TRUE, fastDraw);
		if (!viewBase)
			pView->removeBase();
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnProcessingSubtractbaseline()
{
	if (dat.train.empty())
			return;
	if (!viewBase)
		AfxGetApp()->DoMessageBox(_T("Baseline not yet calculated.\nTo view baseline go to View->Baseline"),0,0);
	else{
		AfxGetApp()->DoWaitCursor(1);
		if (peaksPicked){
			for (unsigned int i = 0; i < dat.train.size(); ++i){
				for (unsigned int j =0; j < dat.train.at(i).peaks.size(); ++j){
					dat.train.at(i).peaks.at(j).intn-=dat.train.at(i).base.at(dat.train.at(i).peaks.at(j).pos);
				}
			}
//			dat.pickPeaks(cutoff,window,peakIter, false);
//			peaksPicked = true;
		}
		dat.subtractBase();
		viewBase=false;
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		pView->clearBackups();
		pView->drawSpec(TRUE, fastDraw);
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		pView->removeBase();
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
		viewBase = false;
		CWnd* pMain = AfxGetMainWnd();
		if (pMain != NULL){
			CMenu* pMenu = pMain->GetMenu();
			if (pMenu != NULL && pMenu->GetMenuItemCount() > 0)
				pMenu->CheckMenuItem(ID_VIEW_BASELINE,0);
		}
	}
}

void CSpecAlignDoc::OnSpectrumManualpickpeaks()
{
	if (dat.train.size() == 0)
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!pView->m_Pick){
		pView->m_Pick = true;
	}
	else
		pView->m_Pick = false;
}

void CSpecAlignDoc::OnUpdateSpectrumManualpickpeaks(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Pick )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CSpecAlignDoc::OnSpectrumAutoscale()
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (dat.avg.input.size() != 0){
		pView->avg.spec.AutoRange();
	}
	for (unsigned int i = 0; i < dat.train.size(); ++i){
		pView->spectra[i].spec.AutoRange();
	}

}

void CSpecAlignDoc::OnSpectrumManualremovepeaks()
{
	if (dat.train.size() == 0)
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (!pView->m_Rem){
		pView->m_Rem = true;
	}
	else
		pView->m_Rem = false;
}

void CSpecAlignDoc::OnUpdateSpectrumManualremovepeaks(CCmdUI *pCmdUI)
{
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (pView->m_Rem )
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CSpecAlignDoc::OnSpectrumCropfrom()
{
	if (dat.train.empty())
		return;
	dlgCrop cDlg;
	cDlg.lo = dat.mz_val.front();
	cDlg.up = dat.mz_val.back();
	if (cDlg.DoModal() == IDOK){
		if (cDlg.lo < dat.mz_val.front() || cDlg.up > dat.mz_val.back())
			return;
		double a = cDlg.lo;
		double b = cDlg.up;
		int startPos = dat.getXIndex(cDlg.lo);
		int endPos = dat.getXIndex(cDlg.up);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		dat.cropSpec(startPos,endPos);
		if (!dat.avg.input.empty()){
			pView->avg.updateCropRange();
			showAvg= true;
		}
		for (unsigned int j = 0; j < dat.train.size(); ++j){
			pView->spectra[j].updateCropRange();
		}
		bool vState = pView->vAll;
		pView->vAll = false;
		pView->drawSpec(TRUE, fastDraw);
		pView->vAll = vState;
		pView->allSpec.updateCropRange();
		pView->viewAllSpec(fastDraw);
	}
}



void CSpecAlignDoc::OnProcessingRescaletopositive()
{
	AfxGetApp()->DoWaitCursor(1);
	dat.scalePositive();
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	//if (dat.train.at(0).peaks.size() != 0){
	//	dat.pickPeaks(cutoff,window,peakIter, false);
	//	peaksPicked = true;
	//}
	pView->clearBackups();
	pView->drawSpec(TRUE, fastDraw);
	if (!viewBase)
		pView->removeBase();
	pView->viewAllSpec(fastDraw);
	pView->drawAnno();
	UpdateAllViews(NULL);
	AfxGetApp()->DoWaitCursor(-1);
}

void CSpecAlignDoc::OnProcessingRemovenegative()
{
	AfxGetApp()->DoWaitCursor(1);
	dat.removeNegative();
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	//if (dat.train.at(0).peaks.size() != 0){
	//	dat.pickPeaks(cutoff,window,peakIter, false);
	//	peaksPicked = true;
	//}
	pView->clearBackups();
	pView->drawSpec(TRUE, fastDraw);
	if (!viewBase)
		pView->removeBase();
	pView->viewAllSpec(fastDraw);
	pView->drawAnno();
	UpdateAllViews(NULL);
	AfxGetApp()->DoWaitCursor(-1);
}

void CSpecAlignDoc::OnToolsCombinespectra()
{
	if (dat.train.empty())
		return;
	DlgComb cDlg;
	if (cDlg.DoModal() == IDOK){
		CString c = cDlg.samples;
		CString tmp;
		int curPos = 0;
		vector <int> cVals;
		tmp = c.Tokenize(",",curPos);
		while (tmp != "")
		{
			if (tmp.Find('-',0) != -1){
				int nPos = 0;
				CString start = tmp.Tokenize("-",nPos);
				CString end = tmp.Tokenize("-",nPos);
				LPTSTR lpsz = new TCHAR[start.GetLength()+1];
				_tcscpy(lpsz, start);
				int st = atoi(lpsz);
				lpsz = new TCHAR[end.GetLength()+1];
				_tcscpy(lpsz, end);
				int ed = atoi(lpsz);
				for (int i = st; i <= ed; ++i){
					cVals.push_back(i);
				}
			}
			else{
				LPTSTR lpsz = new TCHAR[tmp.GetLength()+1];
				_tcscpy(lpsz, tmp);
				cVals.push_back(atoi(lpsz));
			}
			tmp = c.Tokenize(",",curPos);
		}
		for (unsigned int i = 0; i < cVals.size(); ++i){
			if (cVals.at(i)-1 < 0 || cVals.at(i)-1 >= dat.train.size()){
				AfxGetApp()->DoMessageBox(_T("Specified spectrum does not exist"),0,0);
				return;
			}
		}
		AfxGetApp()->DoWaitCursor(1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		dat.combineSpec(cVals);

		//for (unsigned int i = 0; i < rVals.size(); ++i){
		//	dat.removeSpec(rVals.at(i)-1);
		//	pView->rmSpec(rVals.at(i)-1);
		//	for (unsigned int j = i+1; j < rVals.size(); ++j){
		//		if (rVals.at(j) > rVals.at(i))
		//			rVals.at(j)--;
		//	}
		//}
		if (dat.train.size() != 0){
			//dat.sumSpec();
			if (!showPeaks)
				pView->drawSpec(TRUE, fastDraw);
			else
				pView->drawPeaks();
			if (dat.train.at(0).peaks.size() != 0){
				//OnSpectrumRemovepeaks();
				//dat.pickPeaks(cutoff,window,peakIter);
				pView->drawAnno();
			}
			pView->viewAllSpec(fastDraw);
		}
		else{
			pView->avg.ShowWindow(SW_HIDE);
		}
		CRect size;
		pView->GetWindowRect(&size);
		pView->updatePane(size.Width()-20);
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnToolsSubtractspectra()
{
	if (dat.train.empty())
		return;
	subtractDlg sDlg;
	if (sDlg.DoModal() == IDOK){
		AfxGetApp()->DoWaitCursor(1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		if (sDlg.spec1 == -1)
			dat.subtractAvgSpec(sDlg.spec2);
		else
			dat.subtractSpec(sDlg.spec1,sDlg.spec2);

		//for (unsigned int i = 0; i < rVals.size(); ++i){
		//	dat.removeSpec(rVals.at(i)-1);
		//	pView->rmSpec(rVals.at(i)-1);
		//	for (unsigned int j = i+1; j < rVals.size(); ++j){
		//		if (rVals.at(j) > rVals.at(i))
		//			rVals.at(j)--;
		//	}
		//}
		if (dat.train.size() != 0){
			//dat.sumSpec();
			if (!showPeaks)
				pView->drawSpec(TRUE, fastDraw);
			else
				pView->drawPeaks();
			if (!viewBase)
				pView->removeBase();
			if (dat.train.at(0).peaks.size() != 0){
				//OnSpectrumRemovepeaks();
				//dat.pickPeaks(cutoff,window,peakIter);
				pView->drawAnno();
			}
			pView->viewAllSpec(fastDraw);
		}
		else{
			pView->avg.ShowWindow(SW_HIDE);
		}
		CRect size;
		pView->GetWindowRect(&size);
		pView->updatePane(size.Width()-20);
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}		
}

void CSpecAlignDoc::OnToolsComparespectra()
{
	if (dat.train.empty())
		return;
	Dlgcompare cDlg;
	resCompDlg rDlg;
	vector <double> res;
	vector <LPTSTR> label;
	if (cDlg.DoModal() == IDOK){
		if (cDlg.EU){
			res.push_back(dat.cal_dist(cDlg.spec1-1,cDlg.spec2-1));
			label.push_back("Euclidean distance");
		}
		if (cDlg.SI){
			res.push_back(dat.cal_si(cDlg.spec1-1,cDlg.spec2-1));
			label.push_back("Similarity index");
		}
		if (cDlg.CC){
			res.push_back(dat.cal_R(cDlg.spec1-1,cDlg.spec2-1));
			label.push_back("Correlation coefficient");
		}
		if (cDlg.SCA){
			res.push_back(dat.cal_sca(cDlg.spec1-1,cDlg.spec2-1));
			label.push_back("Spectral contrast angle");
		}
		if (cDlg.SCAW){
			char b[256];
			res.push_back(dat.cal_partition_sca(cDlg.spec1-1,cDlg.spec2-1,cDlg.win_size));
			_itoa(cDlg.win_size,b,10);
			CString scaw(b);
			scaw.Insert(0,"SCA with window = ");
			char tmp[50]="";
			strcpy(tmp,scaw.GetBuffer(scaw.GetLength()));
			label.push_back(tmp);
			//label.push_back(strcat("SCA with window ",(const char*)itoa(cDlg.win_size,b,10)));
		}
	}
	else
		return;

	rDlg.lsize = res.size();
	for (unsigned int i = 0; i < res.size(); ++i){
		rDlg.res[i] = res[i];
		rDlg.label[i] = label[i];
	}
	if (rDlg.DoModal() == IDOK){

	}
}

void CSpecAlignDoc::OnToolsCompareall()
{
	if (dat.train.empty())
		return;
	Dlgcompare cDlg;
	if (cDlg.DoModal() == IDOK){
		int tests;
		tests = cDlg.EU+cDlg.SI+cDlg.CC+cDlg.SCA+cDlg.SCAW;
		tests *= dat.train.size();
		CProgressBar pBar(_T("Running comparisons"),20,tests);
		if (cDlg.EU){
			ofstream euf("comp_eu.csv");
			//euf<<"Spectrum,";
			//for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	euf<<dat.train.at(i).name.GetBuffer();
			//	if (i<dat.train.size()-1)
			//		euf<<",";
			//	else
			//		euf<<endl;
			//}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	euf<<dat.train.at(i).name.GetBuffer()<<",";
				pBar.StepIt();
				for (unsigned int j = 0; j < dat.train.size();++j){
					euf<<dat.cal_dist(i,j);
					if (j<dat.train.size()-1)
						euf<<",";
					else
						euf<<endl;
				}
			}			
		}
		if (cDlg.SI){
			ofstream esi("comp_si.csv");
			//esi<<"Spectrum,";
			//for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	esi<<dat.train.at(i).name.GetBuffer();
			//	if (i<dat.train.size()-1)
			//		esi<<",";
			//	else
			//		esi<<endl;
			//}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	esi<<dat.train.at(i).name.GetBuffer()<<",";
				pBar.StepIt();
				for (unsigned int j = 0; j < dat.train.size();++j){
					esi<<dat.cal_si(i,j);
					if (j<dat.train.size()-1)
						esi<<",";
					else
						esi<<endl;
				}
			}			
		}
		if (cDlg.CC){
			ofstream ccf("comp_cc.csv");
			//ccf<<"Spectrum,";
			//for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	ccf<<dat.train.at(i).name.GetBuffer();
			//	if (i<dat.train.size()-1)
			//		ccf<<",";
			//	else
			//		ccf<<endl;
			//}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	ccf<<dat.train.at(i).name.GetBuffer()<<",";
				pBar.StepIt();
				for (unsigned int j = 0; j < dat.train.size();++j){
					ccf<<1-dat.cal_R(i,j);
					if (j<dat.train.size()-1)
						ccf<<",";
					else
						ccf<<endl;
				}
			}			
		}
		if (cDlg.SCA){
			ofstream scaf("comp_sca.csv");
			//scaf<<"Spectrum,";
			//for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	scaf<<dat.train.at(i).name.GetBuffer();
			//	if (i<dat.train.size()-1)
			//		scaf<<",";
			//	else
			//		scaf<<endl;
			//}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
				//scaf<<dat.train.at(i).name.GetBuffer()<<",";
				pBar.StepIt();
				for (unsigned int j = 0; j < dat.train.size();++j){
					scaf<<dat.cal_sca(i,j);
					if (j<dat.train.size()-1)
						scaf<<",";
					else
						scaf<<endl;
				}
			}			
		}
		if (cDlg.SCAW){
			CString s("comp_scaw_.csv");
			char buf[100];
			s.Insert(10,itoa(cDlg.win_size,buf,10));
			ofstream scawf(s.GetBuffer());
			//scaf<<"Spectrum,";
			//for (unsigned int i = 0; i < dat.train.size(); ++i){
			//	scaf<<dat.train.at(i).name.GetBuffer();
			//	if (i<dat.train.size()-1)
			//		scaf<<",";
			//	else
			//		scaf<<endl;
			//}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
				//scaf<<dat.train.at(i).name.GetBuffer()<<",";
				pBar.StepIt();
				for (unsigned int j = 0; j < dat.train.size();++j){
					scawf<<dat.cal_partition_sca(i,j,cDlg.win_size);
					if (j<dat.train.size()-1)
						scawf<<",";
					else
						scawf<<endl;
				}
			}			
		}
	}		
}

void CSpecAlignDoc::OnProcessingSpectraalignmentbybestfit()
{
	if (dat.train.empty())
		return;
	alignfitDlg aDlg;
	aDlg.start = dat.mz_val.front();
	aDlg.end = dat.mz_val.back();
	//aDlg.segs = (int)ceil(dat.mz_val.size()/500);
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	aDlg.lower = pView->spectra[0].m_minx;
	aDlg.upper = pView->spectra[0].m_maxx;
	//aDlg.segs = (int)ceil((dat.getXIndex(aDlg.upper)-dat.getXIndex(aDlg.lower))/500);
	aDlg.segsize = dat.mz_val.size()/20;
	if (aDlg.DoModal() == IDOK){
		if (aDlg.ref == 0 && dat.avg.input.empty()){
			AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
			return;
		}
		if (aDlg.lower> aDlg.upper){
			double tmp = aDlg.lower;
			aDlg.lower=aDlg.upper;
			aDlg.upper= tmp;
		}
		if (aDlg.segsize == 0)
			aDlg.segsize = 100;
		if ((int)aDlg.lower< (int)aDlg.start || (int)aDlg.upper> (int)aDlg.end){
			AfxGetApp()->DoMessageBox(_T("Range specified exceeds limits of dataset"),0,0);
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
//		dat.preshiftSpec(aDlg.win,aDlg.segs,aDlg.fitMethod,aDlg.lower,aDlg.upper);
		dat.pafftAlign(aDlg.segsize,aDlg.lower,aDlg.upper,aDlg.scale,aDlg.win,aDlg.ref-1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		if (dat.train.at(0).peaks.size() != 0){
			//OnSpectrumRemovepeaks();
			dat.pickPeaks(cutoff,window,height, true);
			peaksPicked = true;
		}
		if (showPeaks){
			pView->drawPeaks();
		}
		else
			pView->drawSpec(FALSE, fastDraw);
		if (!viewBase)
			pView->removeBase();
		if (barGraph){
			if (!dat.avg.input.empty()){
				pView->avg.spec.SetElementLinetype(9);
			}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
				pView->spectra[i].spec.SetElementLinetype(9);
			}
		}
			
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnSpectraalignmentCombinedmethod()
{
	if (dat.train.empty())
		return;

	alignCombineDlg aDlg;
	aDlg.start = dat.mz_val.front();
	aDlg.end = dat.mz_val.back();
	//aDlg.segs = (int)ceil(dat.mz_val.size()/500);
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	aDlg.lower = pView->spectra[0].m_minx;
	aDlg.upper = pView->spectra[0].m_maxx;
	//aDlg.segs = (int)ceil((dat.getXIndex(aDlg.upper)-dat.getXIndex(aDlg.lower))/500);
	if (aDlg.DoModal() == IDOK){
		if (aDlg.ref == 0 && dat.avg.input.empty()){
			AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
			return;
		}
		if (aDlg.lower> aDlg.upper){
			double tmp = aDlg.lower;
			aDlg.lower=aDlg.upper;
			aDlg.upper= tmp;
		}
		//if (aDlg.segs == 0)
		//	aDlg.segs = 1;
		if ((float)aDlg.lower< (float)aDlg.start || (float)aDlg.upper> (float)aDlg.end){
			AfxGetApp()->DoMessageBox(_T("Range specified exceeds limits of dataset"),0,0);
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
		dat.rafftAlign(aDlg.lower,aDlg.upper,aDlg.scale,aDlg.shift,aDlg.lookahead,aDlg.ref-1);
		dat.clearPeaks(true);
		dat.pickPeaks(0.01,21,1,true);
		dat.alignPeaks(aDlg.shift,0.01,21,1,aDlg.lower,aDlg.upper);
		dat.clearPeaks(false);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		if (peaksPicked){
			//OnSpectrumRemovepeaks();
			dat.pickPeaks(cutoff,window,height, true);
			peaksPicked = true;
		}
		if (showPeaks){
			pView->drawPeaks();
		}
		else
			pView->drawSpec(FALSE, fastDraw);
		if (!viewBase)
			pView->removeBase();
		if (barGraph){
			if (!dat.avg.input.empty()){
				pView->avg.spec.SetElementLinetype(9);
			}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
				pView->spectra[i].spec.SetElementLinetype(9);
			}
		}
			
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnSpectraalignmentChoice()
{
	if (dat.train.empty())
		return;
	if (dat.avg.input.empty()){
		AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
		return;
	}
	alignChoiceDlg aDlg;
	if (aDlg.DoModal() == IDOK){
		if (aDlg.choice == 0)
			this->OnProcessingAlignpeaks();			
		else if (aDlg.choice == 1)
			this->OnProcessingSpectraalignmentbybestfit();
		else if (aDlg.choice == 2)
			this->OnSpectraalignmentFftmethod();
		else
			this->OnSpectraalignmentCombinedmethod();
	}
}

void CSpecAlignDoc::OnViewFastdraw()
{
	if (fastDraw)
		fastDraw = false;
	else
		fastDraw = true;
}

void CSpecAlignDoc::OnUpdateViewFastdraw(CCmdUI *pCmdUI)
{
	if(fastDraw)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnToolsGeneratecorrelelograph()
{
	if (dat.train.empty())
		return;
	if (dat.avg.input.empty()){
		AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
		return;
	}
	dlgCorr cDlg;
	if (cDlg.DoModal() == IDOK){
		AfxGetApp()->DoWaitCursor(1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		dat.genCorr(cDlg.win);
		if (dat.train.size() != 0){
			//dat.sumSpec();
			if (!showPeaks)
				pView->drawSpec(TRUE, fastDraw);
			else
				pView->drawPeaks();
			if (!viewBase)
				pView->removeBase();
			if (dat.train.at(0).peaks.size() != 0){
				//OnSpectrumRemovepeaks();
				//dat.pickPeaks(cutoff,window,peakIter);
				pView->drawAnno();
			}
			pView->viewAllSpec(fastDraw);
		}
		else{
			pView->avg.ShowWindow(SW_HIDE);
		}
		CRect size;
		pView->GetWindowRect(&size);
		pView->updatePane(size.Width()-20);
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}		
}

void CSpecAlignDoc::OnSpectraalignmentFftmethod()
{
	if (dat.train.empty())
		return;
	fftADlg fDlg;
	fDlg.start = dat.mz_val.front();
	fDlg.end = dat.mz_val.back();
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	fDlg.low = pView->spectra[0].m_minx;
	fDlg.high = pView->spectra[0].m_maxx;
	if (fDlg.DoModal() == IDOK){
		if (fDlg.ref == 0 && dat.avg.input.empty()){
			AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
			return;
		}
		if (fDlg.low> fDlg.high){
			double tmp = fDlg.low;
			fDlg.low=fDlg.high;
			fDlg.high= tmp;
		}
		if ((float)fDlg.low< (float)fDlg.start || (float)fDlg.high> (float)fDlg.end){
			AfxGetApp()->DoMessageBox(_T("Range specified exceeds limits of dataset"),0,0);
			return;
		}
		AfxGetApp()->DoWaitCursor(1);
		dat.rafftAlign(fDlg.low,fDlg.high,fDlg.scale,fDlg.shift,fDlg.lookahead, fDlg.ref-1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		if (dat.train.at(0).peaks.size() != 0){
			//OnSpectrumRemovepeaks();
			dat.pickPeaks(cutoff,window,height, true);
			peaksPicked = true;
		}
		if (showPeaks){
			pView->drawPeaks();
		}
		else
			pView->drawSpec(FALSE, fastDraw);
		if (!viewBase)
			pView->removeBase();
		if (barGraph){
			if (!dat.avg.input.empty()){
				pView->avg.spec.SetElementLinetype(9);
			}
			for (unsigned int i = 0; i < dat.train.size(); ++i){
				pView->spectra[i].spec.SetElementLinetype(9);
			}
		}
			
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnExportPeaks()
{
	if (dat.train.empty())
		return;
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv";
	CFileDialog fDlg(FALSE,"*.csv","*.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select export peaks filename");
	if (fDlg.DoModal() == IDOK && fDlg.GetFileName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFileName();
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		dat.exportPeaks(lpsz,this->cutoff,this->window,this->height);
		AfxGetApp()->DoWaitCursor(-1);
	}
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (showPeaks)
		pView->drawPeaks();
	pView->drawAnno();
	peaksPicked = true;
}

void CSpecAlignDoc::OnExportPeakarea()
{
	if (dat.train.empty())
		return;
	if (!viewPModel){
		OnSpectrumCalculatepeakareas();
		if (!viewPModel)
			return;
	}
	areaDlg aDlg;
	aDlg.percent = 68;
	if (aDlg.DoModal() == IDOK){
		if (aDlg.percent < 0 || aDlg.percent > 100){
			AfxGetApp()->DoMessageBox(_T("Percentage cannot be less than 0 or greater than 100"),0,0);
			return;
		}
		static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv";
		CFileDialog fDlg(FALSE,"*.csv","*.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,sizeof(OPENFILENAME));
		fDlg.m_pOFN->lpstrTitle = CString("Select export peaks filename");
		if (fDlg.DoModal() == IDOK && fDlg.GetFileName() != ""){
			AfxGetApp()->DoWaitCursor(1);
			CString path = fDlg.GetFileName();
			LPTSTR lpsz = new TCHAR[path.GetLength()+1];
			_tcscpy(lpsz, path);
			dat.exportPeakAreas(lpsz, aDlg.percent);
			AfxGetApp()->DoWaitCursor(-1);
		}
	}
	//POSITION pos = GetFirstViewPosition();
	//CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	//if (showPeaks)
	//	pView->drawPeaks();
	//pView->drawAnno();
	//peaksPicked = true;
}

void CSpecAlignDoc::OnViewBaseline()
{
	if (dat.train.empty())
		return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (viewBase){
		if (!dat.avg.input.empty())
			dat.avg.base.clear();
		for (unsigned int i = 0; i < dat.train.size(); ++i){
			dat.train.at(i).base.clear();
		}
		pView->removeBase();
		viewBase = false;
	}
	else{
		subDlg sDlg;
		sDlg.subVal = baseVal;
		if (sDlg.DoModal() == IDOK){
			baseVal = sDlg.subVal;
			dat.calBase(sDlg.subVal);
			viewBase = true;
			if (showPeaks)
				pView->drawPeaks();
			pView->drawBase();
			pView->viewAllSpec(fastDraw);
			pView->drawAnno();
			UpdateAllViews(NULL);
		}
	}
}

void CSpecAlignDoc::OnUpdateViewBaseline(CCmdUI *pCmdUI)
{
	if(viewBase)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CSpecAlignDoc::OnSpectrumCalculatepeakareas()
{
	if (dat.train.empty())
	return;
	if (dat.avg.input.empty()){
		AfxGetApp()->DoMessageBox(_T("Please generate or import an average spectrum first.\n\n(To generate, look under spectrum menu)"),0,0);
		return;
	}
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	if (dat.train.at(0).base.empty()){
		int state = AfxGetApp()->DoMessageBox(_T("No baseline selected, Do you want SpecAlign to pick baseline automatically?\n\nIt is highly recommended that a baseline is picked before modelling peaks."),MB_YESNOCANCEL|MB_ICONEXCLAMATION|MB_APPLMODAL,0);
		if (state == IDYES){
			dat.calBase(10);
			baseVal = 10;
			viewBase = true;
			pView->drawBase();
		}
		else if (state == IDCANCEL)
			return;
	}
	if (dat.train.at(0).peaks.empty()){
		int state = AfxGetApp()->DoMessageBox(_T("No peaks picked. Do you want SpecAlign to pick peaks automatically?"),MB_YESNOCANCEL|MB_ICONEXCLAMATION|MB_APPLMODAL,0);
		if (state == IDYES){
			dat.pickPeaks(cutoff,window ,height,true);
			peaksPicked = true;
			pView->drawAnno();
		}
		else if (state == IDCANCEL || state == IDNO)
			return;
	}
	modelDlg mDlg;
	mDlg.model_type = model;
	if (mDlg.DoModal() == IDOK){
		AfxGetApp()->DoWaitCursor(1);
		dat.modelPeaks(mDlg.model_type,baseVal);
		model = mDlg.model_type;
		viewPModel = true;
		pView->removeModels();
		pView->drawModels();
	//	pView->viewAllSpec(fastDraw);
	//	UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
	return;
}



void CSpecAlignDoc::OnProcessingDenoise()
{
	if (dat.train.empty())
			return;
	denoiseDlg dDlg;
	dDlg.sens = denoise_sens;
	if (dDlg.DoModal() == IDOK){
		denoise_sens = dDlg.sens;
		AfxGetApp()->DoWaitCursor(1);
		vector <double> oldppos;
		for (unsigned int i = 0; i < dat.train.at(0).peaks.size(); ++i){
			oldppos.push_back(dat.train.at(0).peaks.at(i).mz);
		}
		dat.waveDenoise(1-denoise_sens,1);
		POSITION pos = GetFirstViewPosition();
		CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
		dat.clearPeaks(false);
		for (unsigned int i = 0; i < oldppos.size(); ++i){
			dat.addPeak(oldppos.at(i), false);
		}
		pView->drawSpec(FALSE, fastDraw);
		if (!viewBase)
			pView->removeBase();
		pView->viewAllSpec(fastDraw);
		pView->drawAnno();
		UpdateAllViews(NULL);
		AfxGetApp()->DoWaitCursor(-1);
	}
}

void CSpecAlignDoc::OnToolsEvaluatedataset()
{
	if (dat.train.empty())
			return;
	POSITION pos = GetFirstViewPosition();
	CSpecAlignView* pView = (CSpecAlignView*) GetNextView( pos );
	evalDlg eDlg;
	eDlg.start = pView->spectra[0].m_minx;
	eDlg.end = pView->spectra[0].m_maxx;
	eDlg.segments = eval_segs;
	if (eDlg.DoModal() == IDOK){
		static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv";
		CFileDialog fDlg(FALSE,"*.csv","*.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,sizeof(OPENFILENAME));
		fDlg.m_pOFN->lpstrTitle = CString("Select dataset evaluation output filename");
		if (fDlg.DoModal() == IDOK && fDlg.GetFileName() != ""){
			AfxGetApp()->DoWaitCursor(1);
			CString path = fDlg.GetFileName();
			LPTSTR lpsz = new TCHAR[path.GetLength()+1];
			_tcscpy(lpsz, path);
			int status = dat.evaluateDataset(lpsz,eDlg.start,eDlg.end,eDlg.segments);
			eval_segs =eDlg.segments;
			if (status == -1)
				AfxGetApp()->DoMessageBox(_T("More than one spectrum must be loaded for the dataset to be evaluated."),0,0);
		}
	}
}

void CSpecAlignDoc::OnExportSpectramodel()
{
	if (dat.train.empty())
		return;
	static char BASED_CODE szFilter[] = "CSV Files (*.csv)|*.csv";
	CFileDialog fDlg(FALSE,"*.csv","*.csv",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL,sizeof(OPENFILENAME));
	fDlg.m_pOFN->lpstrTitle = CString("Select filename");
	if (fDlg.DoModal() == IDOK && fDlg.GetFileName() != ""){
		AfxGetApp()->DoWaitCursor(1);
		CString path = fDlg.GetFileName();
		LPTSTR lpsz = new TCHAR[path.GetLength()+1];
		_tcscpy(lpsz, path);
		dat.exportModeledSpectra(lpsz);
		AfxGetApp()->DoWaitCursor(-1);
	}
}
