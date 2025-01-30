#pragma once
#include "ntgraphctrl1.h"
#include <vector>

// fullView dialog
using namespace std;

class fullView : public CDialog
{
	DECLARE_DYNAMIC(fullView)

public:
	fullView(CWnd* pParent = NULL);   // standard constructor
	virtual ~fullView();

	void setRange(double minx, double maxx, double miny, double maxy);
	void getRange(double &minx, double &maxx, double &miny, double &maxy);
	void revertRange(bool rel);
	void updateCropRange();
	void undoRange();

// Dialog Data
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CNtgraphctrl1 spec;
	bool created;
	double fullMaxY;
	double fullMinY;

	double m_minx;
	double m_maxx;
	double m_miny;
	double m_maxy;
	vector <double> m_ominx;
	vector <double> m_omaxx;
	vector <double> m_ominy;
	vector <double> m_omaxy;
	double m_bminx;
	double m_bmaxx;
	double m_bminy;
	double m_bmaxy;
};
