// ChildFrm.h : interface of the CChildFrame class
//


#pragma once


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:
	static void Initialize(); 
	static void Terminate();
// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG

	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void ActivateFrame(int nCmdShow); 
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	static int m_nDefCmdShow; 
	static int m_nDefCmdShowOld; 
//	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
