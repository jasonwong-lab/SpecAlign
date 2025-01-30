// SpecAlignView.h : interface of the CSpecAlignView class
//


#pragma once

#include "ntspec.h"
#include "fullView.h"
#include "SpecAlignDoc.h"

class CSpecAlignView : public CScrollView
{
protected: // create from serialization only
	CSpecAlignView();
	DECLARE_DYNCREATE(CSpecAlignView)

// Attributes
public:
	CSpecAlignDoc* GetDocument() const;

	fullView allSpec;
	ntspec spectra[5000];
	//vector <ntspec> spectra;
	ntspec avg;
	bool displayAvg;
	bool m_Track;
	bool m_Zoom;
	bool m_Crop;
	bool m_Pick;
	bool m_Rem;
	double lrange;
	bool vAll;
	bool fDraw;
private:
	bool created;
	unsigned int numSpec;
	unsigned int tnumSpec;


// Operations
public:
	void DoZoom(CPoint point);
	void DoPan(CPoint point);
	void DoPick(CPoint pt);
	void UnZoom();
	void DoCursor();
	void clearBackups();
	void viewAllSpec(bool fastDraw);
	
// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView *pSender,LPARAM lHint, CObject *pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSpecAlignView();

	int updateScroll();
	void updatePane(int cx);
	void updateSrn(int cx, int cy);
	void drawSpec(bool reRange, bool fastDraw);
	void drawAnno();
	void drawPeaks();
	void drawBase();
	bool drawModels();
	void rmSpec(int pos);
	void moveSpec(int oldp, int newp);
	void setUnStackVisible();
	void createAvg();
	void removeBase();
	void removeModels();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void createSpec();
// Generated message map functions
protected:
	//{{AFX_MSG(CSpecAlignView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};

#ifndef _DEBUG  // debug version in SpecAlignView.cpp
inline CSpecAlignDoc* CSpecAlignView::GetDocument() const
   { return reinterpret_cast<CSpecAlignDoc*>(m_pDocument); }
#endif

