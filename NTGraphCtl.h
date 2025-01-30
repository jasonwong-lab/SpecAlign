/////////////////////////////////////////////////////////////////////////////
// NTGraphCtl.h : Declaration of the CNTGraphCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
// CNTGraphCtrl : See NTGraphCtl.cpp for implementation.
/////////////////////////////////////////////////////////////////////////////
//
// Autor : Nikolai Teofilov  nteofilov@yahoo.de
//			
// Comment	: Use with your own risk !
//
// Copyright (c) 2003.
//
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_NTGRAPHCTL_H__5991E2FF_DB47_484E_93AC_35BAE9A2B382__INCLUDED_)
#define AFX_NTGRAPHCTL_H__5991E2FF_DB47_484E_93AC_35BAE9A2B382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////////////////////////
//Line style constants
typedef enum LineType {Solid, Dash, Dot, DashDot, DashDotDot, Null, XYStep, YXStep, Bars, Stick};
/////////////////////////////////////////////////////////////////////////////////////////////////
//Enum to describe symbol style constants
typedef enum SymbolType {Nosym, Dots, Rectangles, Diamonds,	Asterisk, 
	            DownTriangles, UpTriangles, LeftTriangles, RightTriangles};

typedef enum CursorMode {Fixed, Floating, Snap};
typedef enum Crosshair  {XY, X, Y};
///////////////////////////////////////////////////////////
// Declaration of the CElementPoint class.
class CElementPoint
{

public:
   double x, y;
   CElementPoint () { x=y=0; }
   CElementPoint (double c1, double c2)
   {
      x = c1;      y = c2;    
   }
   CElementPoint& operator=(const CElementPoint& pt)
   {
      x = pt.x;  y = pt.y;
      return *this;
   }
   CElementPoint (const CElementPoint& pt)
   {
      *this = pt;
   }
};


///////////////////////////////////////////////////////////
// Declaration of the CGraphAnnotation class.
class CGraphCursor 
{

public:
	OLE_COLOR m_Color;
	BOOL m_bVisible;
	CursorMode m_nMode;
	CElementPoint position;
	Crosshair m_nStyle;

	CGraphCursor () 
	{
		m_Color = RGB(255,255,255);
		m_bVisible = TRUE;
		position = CElementPoint(0,0);
		m_nStyle  = XY;
		m_nMode = Fixed;
	}
};
///////////////////////////////////////////////////////////
// Declaration of the CGraphAnnotation class.
class CGraphAnnotation 
{
public:
	CString   m_Caption;
	OLE_COLOR m_Color, m_BkColor;
	BOOL m_bHorizontal;
	BOOL m_bVisible;
	CElementPoint place;

	CGraphAnnotation () 
	{
		m_Caption = _T("");
		m_Color = RGB(255,255,255);
		m_BkColor = RGB(120,120,120);
		m_bHorizontal = TRUE;
		m_bVisible = TRUE;
		place = CElementPoint(0.5,0.5);
	}
};

///////////////////////////////////////////////////////////
// Declaration of the CGraphElement class.
class CGraphElement : public CObject
{
public:
	CGraphElement(int id=0)
	{
		bIsPlotAvailable = FALSE ;
		m_LineColor = m_PointColor = RGB(0,0,0);
		m_nType =   Solid;
		m_nWidth =  0 ;
		m_nSymbol =   Solid;
		m_bSolid = TRUE;
		m_nSize =  0 ;
		m_bShow = TRUE;      // Added by A. Hofmann
		m_strName.Format("Element-%d",id);
		min=max=CElementPoint(0,0);		
	}
	BOOL m_bShow; 
	BOOL bIsPlotAvailable ;
	COLORREF m_LineColor ;
	COLORREF m_PointColor ;
	long m_nType;
	long m_nWidth ;
	long m_nSymbol;
	BOOL m_bSolid;
	long m_nSize;
	CString m_strName;
	CElementPoint min, max;
	CList<CElementPoint, CElementPoint&> m_PointList;
};

///////////////////////////////////////////////////////////
// CNTGraphCtrl : See NTGraphCtl.cpp for implementation.

class CNTGraphCtrl : public CWnd
{
	DECLARE_DYNCREATE(CNTGraphCtrl)

// Constructor
public:
	CNTGraphCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNTGraphCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
//	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	virtual void OnFontChanged();
	virtual void OnTextChanged();
	protected:
	virtual void OnDrawMetafile(CDC* pDC, const CRect& rcBounds);		 
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DrawGraphOffScreen(CDC* pDC, const CRect& rcBounds);
	void DrawGraph(CDC* pDC, const CRect& rcBounds);
	void DrawControlFrame(CDC* pDC);
    void DrawCursor(CDC *pDC);
	void DrawTitle(CDC *pDC);
	void DrawGrid(CDC *pDC);
	void DrawAxis(CDC *pDC);
	void DrawTicks(CDC *pDC);
	void DrawAxisLabel(CDC *pDC);
	void DrawXLogGrid(CDC *pDC);
	void DrawYLogGrid(CDC *pDC);
	void PlotElement(CDC *pDC);
	void DrawElementLabel(CDC *pDC, CGraphElement *pElement, int index);
	void PlotElementLines(CDC *pDC, CGraphElement *pElement);
	void PlotElementPoints(CDC *pDC, CGraphElement *pElement);
	void CursorPosition(CPoint point);
	void Annotate(CDC *pDC);
	void ResCalc();
	void CalcRect(CDC *pDC);
	void CreateFont(CDC *pDC);
	CPoint Corrdinate(double x, double y);
    CPoint Corrdinate(CElementPoint pt);
	CElementPoint FindPoint(double x, double y);
	CString FormatXAxisLabel(double data);
	CString FormatYAxisLabel(double data);
	void PrepareForDrawing(CDC *pDC, CRect rect);
    void FillToolInfo(TOOLINFO *ti);
	void DoPan(CPoint point, long mode);
	void DoZoom(UINT nFlags, CPoint point);
	void RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);


   // Enum to describe tracking mode state
   enum TrackModeState {None, Track, Zoom, PanXY , PanX, PanY};

   // Enum to describe control's frame style constants
   enum FrameStyle {Flat, Scope, Bitmap};

   ~CNTGraphCtrl();

	DECLARE_OLECREATE_EX(CNTGraphCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CNTGraphCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CNTGraphCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CNTGraphCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CNTGraphCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
//	//{{AFX_DISPATCH(CNTGraphCtrl)
	OLE_COLOR m_axisColor;
	 void OnAxisColorChanged();
	OLE_COLOR m_gridColor;
	 void OnGridColorChanged();
	OLE_COLOR m_labelColor;
	 void OnLabelColorChanged();
	OLE_COLOR m_cursorColor;
	 void OnCursorColorChanged();
	short m_nGridX;
	 void OnXGridNumberChanged();
	short m_nGridY;
	 void OnYGridNumberChanged();
	BOOL m_showGrid;
	 void OnShowGridChanged();
	CString m_xLabel;
	 void OnXLabelChanged();
	CString m_yLabel;
	 void OnYLabelChanged();
	OLE_COLOR m_elementLineColor;
	 void OnElementLineColorChanged();
	OLE_COLOR m_elementPointColor;
	 void OnElementPointColorChanged();
	long m_elementLinetype;
	 void OnElementLinetypeChanged();
	long m_elementWidth;
	 void OnElementWidthChanged();
	long m_elementPointSymbol;
	 void OnElementPointSymbolChanged();
	BOOL m_elementSolidPoint;
	 void OnElementSolidPointChanged();
	BOOL m_elementShow;
	 void OnElementShowChanged();
	CString m_elementName;
	 void OnElementNameChanged();
	BOOL m_elementIdentify;
	 void OnElementIdentifyChanged();
	BOOL m_xLog;
	 void OnXLogChanged();
	BOOL m_yLog;
	 void OnYLogChanged();
	OLE_COLOR m_controlFrameColor;
	 void OnControlFrameColorChanged();
	OLE_COLOR m_plotAreaColor;
	 void OnPlotAreaColorChanged();
	short m_frameStyle;
	 void OnFrameStyleChanged();
	CString m_annoLabelCaption;
	 void OnAnnoLabelCaptionChanged();
	double m_annoLabelX;
	 void OnAnnoLabelXChanged();
	double m_annoLabelY;
	 void OnAnnoLabelYChanged();
	OLE_COLOR m_annoLabelColor;
	 void OnAnnoLabelColorChanged();
	BOOL m_annoLabelHorizontal;
	 void OnAnnoLabelHorizontalChanged();
	short m_nAnnotation;
	 void OnAnnotationChanged();
	BOOL m_annoVisible;
	 void OnAnnoVisibleChanged();
	short m_nElementID;
	 void OnElementChanged();
	OLE_COLOR m_annoLabelBkColor;
	 void OnAnnoLabelBkColorChanged();
	short m_nCursorID;
	 void OnCursorChanged();
	double m_cursorX;
	 void OnCursorXChanged();
	double m_cursorY;
	 void OnCursorYChanged();
	short m_cursorStyle;
	 void OnCursorStyleChanged();
	BOOL m_cursorVisible;
	 void OnCursorVisibleChanged();
	short m_cursorMode;
	 void OnCursorModeChanged();
	CString m_formatAxisBottom;
	 void OnFormatAxisBottomChanged();
	CString m_formatAxisLeft;
	 void OnFormatAxisLeftChanged();
	BOOL m_yTime;
	 void OnYTimeChanged();
	BOOL m_xTime;
	 void OnXTimeChanged();
	 LPFONTDISP GetLabelFont();
	 void SetLabelFont(LPFONTDISP newValue);
	 LPFONTDISP GetTickFont();
	 void SetTickFont(LPFONTDISP newValue);
	 LPFONTDISP GetTitleFont();
	 void SetTitleFont(LPFONTDISP newValue);
	 LPFONTDISP GetIdentFont();
	 void SetIdentFont(LPFONTDISP newValue);
	 long GetTrackMode();
	 void SetTrackMode(long nNewValue);
	 LPPICTUREDISP GetControlFramePicture();
	 void SetControlFramePicture(LPPICTUREDISP newValue);
	 LPPICTUREDISP GetPlotAreaPicture();
	 void SetPlotAreaPicture(LPPICTUREDISP newValue);
	 short GetAnnoCount();
	 short GetElementCount();
	 short GetCursorCount();
	 void SetRange(double xmin, double xmax, double ymin, double ymax);
	 void AutoRange();
	 void CopyToClipboard();
	 void PrintGraph();
	 void AddElement();
	 void DeleteElement(short ElementID);
	 void ClearGraph();
	 void PlotXY(double X, double Y, short ElementID);
	 void PlotY(double Y, short ElementID);
	 void ShowProperties();
	 void SaveAs(LPCTSTR szFilename);
	 void AddAnnotation();
	 void DeleteAnnotation(short annoID);
	 void AddCursor();
	 void DeleteCursor(short cursorID);
	 double GetXMin();
	 double GetXMax();
	 double GetYMin();
	 double GetYMax();
	 long GetXAxisMinPos();
	 long GetXAxisMaxPos();
	 double GetElementXValue(long index, short ElementID);
	 void SetElementXValue(long index, short ElementID, double newValue);
	 double GetElementYValue(long index, short ElementID);
	 void SetElementYValue(long index, short ElementID, double newValue);
	//}}AFX_DISPATCH
//	DECLARE_DISPATCH_MAP()

//	afx_msg void AboutBox();

// Event maps
//	//{{AFX_EVENT(CNTGraphCtrl)
	void FireCursorPosition(double x, double y);
//		{FireEvent(eventidCursorPosition,EVENT_PARAM(VTS_R8  VTS_R8), x, y);}
	void FireTrackModeChanged(long lNewState);
//		{FireEvent(eventidTrackModeChanged,EVENT_PARAM(VTS_I4), lNewState);}
//	//}}AFX_EVENT
//	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	//enum {
	////{{AFX_DISP_ID(CNTGraphCtrl)
	//dispidAxisColor = 1L,
	//dispidGridColor = 2L,
	//dispidLabelColor = 3L,
	//dispidCursorColor = 4L,
	//dispidLabelFont = 43L,
	//dispidTickFont = 44L,
	//dispidTitleFont = 45L,
	//dispidIdentFont = 46L,
	//dispidXGridNumber = 5L,
	//dispidYGridNumber = 6L,
	//dispidShowGrid = 7L,
	//dispidXLabel = 8L,
	//dispidYLabel = 9L,
	//dispidElementLineColor = 10L,
	//dispidElementPointColor = 11L,
	//dispidElementLinetype = 12L,
	//dispidElementWidth = 13L,
	//dispidElementPointSymbol = 14L,
	//dispidElementSolidPoint = 15L,
	//dispidElementShow = 16L,
	//dispidTrackMode = 47L,
	//dispidElementName = 17L,
	//dispidElementIdentify = 18L,
	//dispidXLog = 19L,
	//dispidYLog = 20L,
	//dispidControlFramePicture = 48L,
	//dispidPlotAreaPicture = 49L,
	//dispidControlFrameColor = 21L,
	//dispidPlotAreaColor = 22L,
	//dispidFrameStyle = 23L,
	//dispidAnnoLabelCaption = 24L,
	//dispidAnnoLabelX = 25L,
	//dispidAnnoLabelY = 26L,
	//dispidAnnoLabelColor = 27L,
	//dispidAnnoLabelHorizontal = 28L,
	//dispidAnnotation = 29L,
	//dispidAnnoVisible = 30L,
	//dispidAnnoCount = 50L,
	//dispidElement = 31L,
	//dispidElementCount = 51L,
	//dispidAnnoLabelBkColor = 32L,
	//dispidCursorCount = 52L,
	//dispidCursor = 33L,
	//dispidCursorX = 34L,
	//dispidCursorY = 35L,
	//dispidCursorStyle = 36L,
	//dispidCursorVisible = 37L,
	//dispidCursorMode = 38L,
	//dispidFormatAxisBottom = 39L,
	//dispidFormatAxisLeft = 40L,
	//dispidYTime = 41L,
	//dispidXTime = 42L,
	//dispidSetRange = 53L,
	//dispidAutoRange = 54L,
	//dispidCopyToClipboard = 55L,
	//dispidPrintGraph = 56L,
	//dispidAddElement = 57L,
	//dispidDeleteElement = 58L,
	//dispidClearGraph = 59L,
	//dispidElementXValue = 74L,
	//dispidElementYValue = 75L,
	//dispidPlotXY = 60L,
	//dispidPlotY = 61L,
	//dispidShowProperties = 62L,
	//dispidSaveAs = 63L,
	//dispidAddAnnotation = 64L,
	//dispidDeleteAnnotation = 65L,
	//dispidAddCursor = 66L,
	//dispidDeleteCursor = 67L,
	//dispidGetXMin = 68L,
	//dispidGetXMax = 69L,
	//dispidGetYMin = 70L,
	//dispidGetYMax = 71L,
	//dispidGetXAxisMinPos = 72L,
	//dispidGetXAxisMaxPos = 73L,
	//eventidCursorPosition = 1L,
	//eventidTrackModeChanged = 2L,
	////}}AFX_DISP_ID
	//};

private:
	CFont m_fontY, m_fontVLabel;
    CFont m_fontTick;
	CFont m_fontTitle;
    CFont m_fontLabel;
    CFont m_fontIdent;

	CPictureHolder    m_picControlFrame; 
	CPictureHolder    m_picPlotArea; 
	CPictureHolder    m_picFrame; // Default bitmap frame (FrameStyle = Bitmap)
	


    CRect m_ctlRect, m_axisRect;
	double dResY, dResX ;
	double dRangeY[2], dRangeX[2];
	double dOldRangeX[2], dOldRangeY[2];
   	double dAutoRangeX[2], dAutoRangeY[2];

	CPen m_axisPen, m_gridPen, m_cursorPen;
	CBrush m_brushPlot, m_brushFrame;

	CPen   m_linePen,  m_pointPen;
	CBrush m_lineBrush, m_pointBrush, m_annoBrush;

	CPoint m_panPoint;
	BOOL bUnzoom;
	BOOL bIsPlotAvailable;
	double RoundUp(double val, int ex);
	int m_nExponentX;
	int m_nExponentY;

	long m_nTrackMode;
	CToolTipCtrl m_ttip;

	CTypedPtrList<CObList, CGraphElement*> m_ElementList; // Element List
	POSITION m_Position;
	BOOL bIsElementAllocated;
	short m_elementCount;

	CList<CGraphAnnotation, CGraphAnnotation&> m_AnnotationList;
	short m_nAnnoCount;

	CList<CGraphCursor, CGraphCursor&> m_CursorList;
	short m_nCursorCount;

public:
//	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTGRAPHCTL_H__5991E2FF_DB47_484E_93AC_35BAE9A2B382__INCLUDED)
