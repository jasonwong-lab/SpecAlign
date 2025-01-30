// SpecAlignDoc.h : interface of the CSpecAlignDoc class
//


#pragma once

#include "iface.h"

class CSpecAlignDoc : public CDocument
{
protected: // create from serialization only
	CSpecAlignDoc();
	DECLARE_DYNCREATE(CSpecAlignDoc)

// Attributes
public:
	iface dat;
	bool showAvg;
	bool lineGraph;
	bool barGraph;
	bool stacked;
	bool overlap;
	bool showPeaks;
	double height;
	double cutoff;
	int window;
	bool peaksPicked;
	bool plotPeaks;
	bool rel;
	int importType;
	bool fastDraw;
	bool viewBase;
	bool viewPModel;
	int baseVal;
	int model;
	int denoise_recurs;
	double denoise_sens;
	CString curpath;
	CString curSpath;
	int eval_segs;
// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSpecAlignDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSize           m_sizeDoc;
	CListCtrl * results;

	CString decompress(CString src);
	void compress(CString src);
public:
	CSize GetDocSize() { return m_sizeDoc; }
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImportFile();
	afx_msg void OnEditUndo();
	afx_msg void OnProcessingNormalizetic();
	afx_msg void OnEditRevert();
	afx_msg void OnProcessingRelativeintensities();
	afx_msg void OnViewAveragespec();
	afx_msg void OnUpdateViewAveragespec(CCmdUI *pCmdUI);
	afx_msg void OnGraphTrack();
	afx_msg void OnUpdateGraphTrack(CCmdUI *pCmdUI);
	afx_msg void OnGraphZoom();
	afx_msg void OnUpdateGraphZoom(CCmdUI *pCmdUI);
	//afx_msg void OnGraphPan();
	//afx_msg void OnUpdateGraphPan(CCmdUI *pCmdUI);
	afx_msg void OnEditUndoZoom();
	afx_msg void OnEditRevertallzoom();
	afx_msg void OnViewasLinegraph();
	afx_msg void OnUpdateViewasLinegraph(CCmdUI *pCmdUI);
	afx_msg void OnViewasBargraph();
	afx_msg void OnUpdateViewasBargraph(CCmdUI *pCmdUI);
	afx_msg void OnSpectrumPickpeaks();
	afx_msg void OnSpectrumRemovepeaks();
	afx_msg void OnProcessingAlignpeaks();
	afx_msg void OnSpectrumPlotpeaks();
	afx_msg void OnUpdateSpectrumPlotpeaks(CCmdUI *pCmdUI);
	afx_msg void OnImportFolder();
	afx_msg void OnSpectrumCrop();
	afx_msg void OnExportFile();
	afx_msg void OnSpectrumRemovespectrum();
	afx_msg void OnExportMultifile();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileSave();
	afx_msg void OnUpdateSpectrumCrop(CCmdUI *pCmdUI);
	afx_msg void OnProcessingbins();
	afx_msg void OnProcessingSmooth();
	afx_msg void OnSpectrumGenerateaveragespectrum();
	afx_msg void OnExportAveragespectrum();
	afx_msg void OnImportAveragespectrum();
	afx_msg void OnViewasStackedgraph();
	afx_msg void OnUpdateViewasStackedgraph(CCmdUI *pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnEditCopy();
	afx_msg void OnSpectrumMergewithaveragespectrum();
	afx_msg void OnProcessingScale();
	afx_msg void OnProcessingSubtractbaseline();
	afx_msg void OnSpectrumManualpickpeaks();
	afx_msg void OnUpdateSpectrumManualpickpeaks(CCmdUI *pCmdUI);
	afx_msg void OnSpectrumAutoscale();
	afx_msg void OnSpectrumManualremovepeaks();
	afx_msg void OnUpdateSpectrumManualremovepeaks(CCmdUI *pCmdUI);
	afx_msg void OnSpectrumCropfrom();
	afx_msg void OnViewasOverlappinggraph();
	afx_msg void OnUpdateViewasOverlappinggraph(CCmdUI *pCmdUI);
	afx_msg void OnProcessingRescaletopositive();
	afx_msg void OnProcessingRemovenegative();
	afx_msg void OnToolsCombinespectra();
	afx_msg void OnToolsSubtractspectra();
	afx_msg void OnToolsComparespectra();
	afx_msg void OnToolsCompareall();
	afx_msg void OnProcessingSpectraalignmentbybestfit();
	afx_msg void OnSpectraalignmentCombinedmethod();
	afx_msg void OnSpectraalignmentChoice();
	afx_msg void OnViewFastdraw();
	afx_msg void OnUpdateViewFastdraw(CCmdUI *pCmdUI);
	afx_msg void OnToolsGeneratecorrelelograph();
	afx_msg void OnSpectraalignmentFftmethod();
	afx_msg void OnExportPeaks();
	afx_msg void OnViewBaseline();
	afx_msg void OnUpdateViewBaseline(CCmdUI *pCmdUI);
	afx_msg void OnSpectrumCalculatepeakareas();
	afx_msg void OnExportPeakarea();
	afx_msg void OnProcessingDenoise();
	afx_msg void OnToolsEvaluatedataset();
	afx_msg void OnExportSpectramodel();
};


