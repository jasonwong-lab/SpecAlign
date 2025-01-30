#pragma once


// alignChoiceDlg dialog

class alignChoiceDlg : public CDialog
{
	DECLARE_DYNAMIC(alignChoiceDlg)

public:
	alignChoiceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~alignChoiceDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG19 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	int choice;
};
