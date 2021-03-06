
// Shape_DrawerDlg.h : header file
//

#pragma once
#include "shapes.h"

#include <iostream>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CShapeDrawerDlg dialog
class CShapeDrawerDlg : public CDialog
{
	bool start_up; //flag to deside if the program is just started
	int shape_type; //flag to deside wich shape type to draw
	bool is_pressed; //flag to deside if LMB is pressed
	bool is_poly; //flag to deside if the process if drawing a polygon is ended
	bool start_mov; //flag to deside if the process of moving an object is ended
	bool cMapI; //flag to deside if color map is displayed
	bool bg_select; //flag to deside if to change back ground color

	short shiftPressed; //flag to deside if LSHIFT is pressed

	CPoint startP, endP; //start point and end point of a shape

	vector<CPoint> dots; //vertexes that lock the shape in a rectangle or vertexes of polygon

	vector<MyShape*> shapes, lastAction; //vectors to shave all pointers to all the shapes that created, for current state and previous state

	COLORREF currentColor, bgColor; //RGB of current color selected, bgColor - background color

	MyShape *selectedShape, **selectedShapeP, *copy;
	//selected shape - pointer to a shape that is selected
	//selected shape pointer - pointer to get te pointer of the selected shape
	//copy - pointer to a copy of the selected shape

	colorMap cMap; //color map object to draw color map

	

// Construction
public:
	CShapeDrawerDlg(CWnd* pParent = NULL);	// standard constructor
	~CShapeDrawerDlg(); //destructor

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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnResetPushMenu();
	afx_msg void OnColorBlackMenu();
	afx_msg void OnColorWhiteMenu();
	afx_msg void OnColorRedMenu();
	afx_msg void OnColorGreenMenu();
	afx_msg void OnColorBlueMenu();
	afx_msg void OnColorYellowMenu();
	afx_msg void OnColorCyanMenu();
	afx_msg void OnColorMagnetaMenu();
	afx_msg void OnOptionsSelectshapeMenu();
	afx_msg void OnEditBringtofrontMenu();
	afx_msg void OnEditDeleteMenu();
	afx_msg void OnEditUndoMenu();
	afx_msg void OnOptionsInvalidate();
	afx_msg void OnColorSelectcolorMenu();
	afx_msg void OnOptionsExitMenu();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditCopyMenu();
	afx_msg void OnEditPasteMenu();
	afx_msg void OnOptionsSaveMenu();
	afx_msg void OnOptionsLoadMenu();
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnColorBackgroundcolorMenu();
};


