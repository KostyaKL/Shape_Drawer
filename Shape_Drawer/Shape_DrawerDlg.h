
// Shape_DrawerDlg.h : header file
//

#pragma once


// CShapeDrawerDlg dialog
class CShapeDrawerDlg : public CDialog
{
	bool start_up;
	int shape_type;
	bool is_pressed;
	bool is_poly;
	CPoint startP, endP;

	//MyShape *shapes[100];
	int items;

// Construction
public:
	CShapeDrawerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHAPE_DRAWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCirclePressMenu();
	afx_msg void OnNewshapeTriangle();
	afx_msg void OnRectanglePressMenu();
	afx_msg void OnPolygonPressMenu();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLinePresseMenu();
};
