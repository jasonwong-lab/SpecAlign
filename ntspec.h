#pragma once
#include "afxwin.h"
//#include "NTGraphCtl.h"
#include <vector>
#include "ntgraphctrl1.h"
#include "data.h"

using namespace std;
// ntspec dialog

class ntspec : public CDialog
{
	DECLARE_DYNAMIC(ntspec)

public:
	ntspec(CWnd* pParent = NULL);   // standard constructor
	virtual ~ntspec();


	void setRange(double minx, double maxx, double miny, double maxy);
	void getRange(double &minx, double &maxx, double &miny, double &maxy);
	void revertRange(bool rel);
	void updateCropRange();
	void undoRange();
	double getMaxY(double xmin, double xmax, int &startPos);
	void copyRanges(ntspec *o);

	void drawSpec(bool reRange, bool fastDraw, int linetype=0);
	void drawAnno();
	void plotPeaks();
	void removeBase();
	void showBase();
	void removeAllModels();
	bool showModels();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CButton label;
	bool plotP;
	bool avgSpec;
	bool created;
	double m_minx;
	double m_maxx;
	double m_miny;
	double m_maxy;
	bool fDraw;
	vector <double> m_ominx;
	vector <double> m_omaxx;
	vector <double> m_ominy;
	vector <double> m_omaxy;
	double m_bminx;
	double m_bmaxx;
	double m_bminy;
	double m_bmaxy;
	data * d;
	int ID;

	int baseLoc;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	CNtgraphctrl1 spec;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};
