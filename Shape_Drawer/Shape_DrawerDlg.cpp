
// Shape_DrawerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Shape_Drawer.h"
#include "Shape_DrawerDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#define PI 3.14159265
#define N_ZERO 0.0000000001

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShapeDrawerDlg dialog



CShapeDrawerDlg::CShapeDrawerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SHAPE_DRAWER_DIALOG, pParent)
{
#ifdef _DEBUG
	oldMemState.Checkpoint();
#endif



	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	start_up = TRUE;
	is_pressed = FALSE;
	is_poly = FALSE;
	start_mov = FALSE;
	cMapI = FALSE;
	shape_type = 0;		//0 - selection
						//1 - line
						//2 - circle
						//3 - triangle
						//4 - rectangle
						//5 - polygon
	shiftPressed = 0;
	currentColor = RGB(0, 0, 0);
	selectedShapeP = NULL;
	selectedShape = NULL;
	copy = NULL;

	dots.clear();
	shapes.clear();
	lastAction.clear();
	startP.SetPoint(0, 0);
	endP.SetPoint(0, 0);

}

CShapeDrawerDlg::~CShapeDrawerDlg() {
	int size;
	size = shapes.size();
	for (int i = 0;i < size;i++) {
		delete shapes[i];
	}
	shapes.clear();

	size = lastAction.size();
	for (int i = 0;i < size;i++) {
		delete lastAction[i];
	}
	lastAction.clear();

	delete selectedShape;
	delete *selectedShapeP;
	delete selectedShapeP;
	delete copy;
}

void CShapeDrawerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShapeDrawerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_NEWSHAPE_LINE1, &CShapeDrawerDlg::OnLinePresseMenu)
	ON_COMMAND(ID_NEWSHAPE_CIRCLE, &CShapeDrawerDlg::OnCirclePressMenu)
	ON_COMMAND(ID_NEWSHAPE_TRIANGLE, &CShapeDrawerDlg::OnNewshapeTriangle)
	ON_COMMAND(ID_NEWSHAPE_RECTANGLE, &CShapeDrawerDlg::OnRectanglePressMenu)
	ON_COMMAND(ID_NEWSHAPE_POLYGON, &CShapeDrawerDlg::OnPolygonPressMenu)
	
	
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_RESET_REST, &CShapeDrawerDlg::OnResetPushMenu)
	ON_COMMAND(ID_COLOR_BLACK, &CShapeDrawerDlg::OnColorBlackMenu)
	ON_COMMAND(ID_COLOR_WHITE, &CShapeDrawerDlg::OnColorWhiteMenu)
	ON_COMMAND(ID_COLOR_RED, &CShapeDrawerDlg::OnColorRedMenu)
	ON_COMMAND(ID_COLOR_GREEN, &CShapeDrawerDlg::OnColorGreenMenu)
	ON_COMMAND(ID_COLOR_BLUE, &CShapeDrawerDlg::OnColorBlueMenu)
	ON_COMMAND(ID_COLOR_YELLOW, &CShapeDrawerDlg::OnColorYellowMenu)
	ON_COMMAND(ID_COLOR_CYAN, &CShapeDrawerDlg::OnColorCyanMenu)
	ON_COMMAND(ID_COLOR_MAGNETA, &CShapeDrawerDlg::OnColorMagnetaMenu)
	ON_COMMAND(ID_OPTIONS_SELECTSHAPE, &CShapeDrawerDlg::OnOptionsSelectshapeMenu)
	ON_COMMAND(ID_EDIT_BRINGTOFRONT, &CShapeDrawerDlg::OnEditBringtofrontMenu)
	ON_COMMAND(ID_EDIT_DELETE, &CShapeDrawerDlg::OnEditDeleteMenu)
	ON_COMMAND(ID_EDIT_UNDO32779, &CShapeDrawerDlg::OnEditUndoMenu)
	ON_COMMAND(ID_OPTIONS_INVALIDATE, &CShapeDrawerDlg::OnOptionsInvalidate)
	ON_COMMAND(ID_COLOR_SELECTCOLOR, &CShapeDrawerDlg::OnColorSelectcolorMenu)
	ON_COMMAND(ID_OPTIONS_EXIT32781, &CShapeDrawerDlg::OnOptionsExitMenu)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_COPY32801, &CShapeDrawerDlg::OnEditCopyMenu)
	ON_COMMAND(ID_EDIT_PASTE32802, &CShapeDrawerDlg::OnEditPasteMenu)
	ON_COMMAND(ID_OPTIONS_SAVE, &CShapeDrawerDlg::OnOptionsSaveMenu)
	ON_COMMAND(ID_OPTIONS_LOAD, &CShapeDrawerDlg::OnOptionsLoadMenu)
	ON_WM_MENUSELECT()
END_MESSAGE_MAP()


// CShapeDrawerDlg message handlers 

BOOL CShapeDrawerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShapeDrawerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShapeDrawerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	if (start_up) {
		
		CBrush myBrush(RGB(255, 0, 204));
		CBrush *oldBrush;

		CPoint top_left(0, 0);
		CRect rect;
		GetClientRect(&rect);
		CPoint bottom_right = rect.BottomRight();
		bottom_right.y -= 50;
		rect.SetRect(top_left, bottom_right);
		AdjustWindowRect(&rect, WS_OVERLAPPED,1);

		
		oldBrush = dc.SelectObject(&myBrush);
		dc.FillRect(rect, &myBrush);
		dc.SelectObject(oldBrush);

		start_up = FALSE;
	}
	CClientDC dcc(this);

	int size = shapes.size();
	for (int i = 0;i < size;i++) {
		shapes[i]->drawMe(&dcc);
	}

	if (cMapI) {
		cMap.drawMe(&dcc);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShapeDrawerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CShapeDrawerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here

}


void CShapeDrawerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (is_pressed) {
		CClientDC dc(this);
		COLORREF oldPen;
		dc.SelectObject(GetStockObject(DC_PEN));
		oldPen = dc.SetDCPenColor(currentColor);

		if (cMapI && cMap.isInside(point)) {
			CClientDC dc(this);
			currentColor = GetPixel(dc, point.x, point.y);
			cMap.selectedColor(&dc, currentColor);
		}
		else {
			CPoint points[3]; //0 - top, 1 - left, 2 - right;
			shiftPressed = GetKeyState(VK_LSHIFT)&0x80;
			if (shiftPressed) {
				shiftPressed = 0;
				int x = point.x - startP.x;
				int y = point.y - startP.y;
				if (shape_type == 1 || shape_type == 5) {
					double angle = atan(x / (y == 0 ? N_ZERO : y)) * 180 / PI;
					if (x > 0 && y > 0) {
						angle = angle + 90;
					}
					else if (x < 0 && y < 0) {
						angle = 360 - angle + 90;
					}
					else if (x < 0 && y>0) {
						angle = 360 + angle + 270;
					}
					else if (x > 0 && y < 0) {
						angle + angle + 270;
					}
					utilityFunctions::angleFix(angle, startP, point);
				}
				else {
					if (x < 0) {
						x *= -1;
					}
					if (y < 0) {
						y *= -1;
					}
					if (x < y) {
						point.y = startP.y + point.x - startP.x;
					}
					else {
						point.x = startP.x + point.y - startP.y;
					}
				}
			}

			switch (shape_type) {
			case 0:
				if (selectedShape) {
					if (start_mov) {
						start_mov = FALSE;
						utilityFunctions::savePrevState(shapes, lastAction);
					}
					endP = point;
					selectedShape->offsetShape(endP - startP);
					startP = endP;
					Invalidate();
				}
				break;
			case 1:
				dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(startP);
				dc.LineTo(endP);
				endP = point;
				dc.MoveTo(startP);
				dc.LineTo(endP);
				break;
			case 2:
				dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(startP);
				dc.Ellipse(startP.x, startP.y, endP.x, endP.y);
				endP = point;
				dc.MoveTo(startP);
				dc.Ellipse(startP.x, startP.y, endP.x, endP.y);
				break;
			case 3:
				dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(startP);
				utilityFunctions::triangle_Vdots(startP, endP, points[0], points[1], points[2]);
				dc.Polygon(points, 3);
				endP = point;
				dc.MoveTo(startP);
				utilityFunctions::triangle_Vdots(startP, endP, points[0], points[1], points[2]);
				dc.Polygon(points, 3);
				break;
			case 4:
				dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(startP);
				dc.Rectangle(startP.x, startP.y, endP.x, endP.y);
				endP = point;
				dc.MoveTo(startP);
				dc.Rectangle(startP.x, startP.y, endP.x, endP.y);
				break;
			case 5:
				dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(startP);
				dc.LineTo(endP);
				endP = point;
				dc.MoveTo(startP);
				dc.LineTo(endP);
				break;
			default:
				break;
			}
			dc.SetDCPenColor(oldPen);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}




void CShapeDrawerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	if (shape_type == 0) {
		is_pressed = TRUE;
		CClientDC dc(this);
		if (cMapI) {
			if (!cMap.isInside(point)) {
				cMapI = FALSE;
				Invalidate();
			}
			else if (selectedShape && cMap.isInside(point)) {
				utilityFunctions::savePrevState(shapes, lastAction);
				selectedShape->updateColor(currentColor);
			}
		}
		else {
			if (selectedShape) {
				selectedShape->unselectShape();
			}
			selectedShapeP = utilityFunctions::shapeSrch(point, shapes, &dc);
			if (selectedShapeP == NULL) {
				selectedShape = NULL;
			}
			else {
				selectedShape = *selectedShapeP;
				Invalidate();
			}
		}

		startP = point;
		start_mov = TRUE;
	}
	else {
		if (cMapI) {
			if (!cMap.isInside(point)) {
				cMapI = FALSE;
				utilityFunctions::savePrevState(shapes, lastAction);
				Invalidate();
			}	
		}
		if (shape_type == 5) {
			if (is_poly == FALSE) {
				is_pressed = TRUE;
				startP = point;
				endP = point;
				is_poly = TRUE;
			}
			else {
				is_pressed = TRUE;

				CClientDC dc(this);
				COLORREF oldPen;
				dc.SelectObject(GetStockObject(DC_PEN));
				oldPen = dc.SetDCPenColor(currentColor);

				dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(startP);
				dc.LineTo(endP);
				endP = point;
				dc.MoveTo(startP);
				dc.LineTo(endP);

				dc.SetDCPenColor(oldPen);
			}
		}
		else {
			is_pressed = TRUE;
			startP = point;
			endP = point;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CShapeDrawerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (is_pressed) {
		is_pressed = NULL;
		if (cMapI && cMap.isInside(point)) {
			CClientDC dc(this);
			currentColor = GetPixel(dc,point.x,point.y);
			cMap.selectedColor(&dc, currentColor);
			if (selectedShape) {
				utilityFunctions::savePrevState(shapes, lastAction);
				selectedShape->updateColor(currentColor);
			}
		}
		else {
			switch (shape_type) {
			case 0:

				break;
			case 1:
				shapes.push_back(new MyLine(startP, endP, currentColor));
				break;
			case 2:
				shapes.push_back(new MyEllipse(startP, endP, currentColor));
				break;
			case 3:
				shapes.push_back(new MyTriangle(startP, endP, currentColor));
				break;
			case 4:
				shapes.push_back(new MyRectangle(startP, endP, currentColor));
				break;
			case 5:
				dots.push_back(startP);
				startP = endP;
				break;
			default:
				break;
			}
			endP = point;

			if (is_poly == FALSE && shape_type != 0) {
				Invalidate();
			}
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CShapeDrawerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (is_poly) {
		utilityFunctions::savePrevState(shapes, lastAction);

		CClientDC dc(this);
		COLORREF oldPen;
		dc.SelectObject(GetStockObject(DC_PEN));
		oldPen = dc.SetDCPenColor(currentColor);

		dc.MoveTo(startP);
		dc.LineTo(dots[0]);
		shapes.push_back(new MyPolygon(dots, currentColor));
		dots.clear();
		is_poly = FALSE;
		is_pressed = NULL;

		dc.SetDCPenColor(oldPen);

		Invalidate();
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CShapeDrawerDlg::OnLinePresseMenu()
{
	// TODO: Add your command handler code here

	shape_type = 1;
	if (selectedShape) {
		selectedShape->unselectShape();
		Invalidate();
	}
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
	selectedShape = NULL;
}

void CShapeDrawerDlg::OnCirclePressMenu()
{
	// TODO: Add your command handler code here
	shape_type = 2;
	if (selectedShape) {
		selectedShape->unselectShape();
		Invalidate();
	}
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
	selectedShape = NULL;
}


void CShapeDrawerDlg::OnNewshapeTriangle()
{
	// TODO: Add your command handler code here
	shape_type = 3;
	if (selectedShape) {
		selectedShape->unselectShape();
		Invalidate();
	}
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
	selectedShape = NULL;


}



void CShapeDrawerDlg::OnRectanglePressMenu()
{
	// TODO: Add your command handler code here
	shape_type = 4;
	if (selectedShape) {
		selectedShape->unselectShape();
		Invalidate();
	}
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
	selectedShape = NULL;

}


void CShapeDrawerDlg::OnPolygonPressMenu()
{
	// TODO: Add your command handler code here
	shape_type = 5;
	if (selectedShape) {
		selectedShape->unselectShape();
		Invalidate();
	}
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
	selectedShape = NULL;

	is_poly = FALSE;
}


void CShapeDrawerDlg::OnResetPushMenu()
{
	// TODO: Add your command handler code here
	Invalidate();
}


void CShapeDrawerDlg::OnColorBlackMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(0, 0, 0);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorWhiteMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(255, 255, 255);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorRedMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(255, 0, 0);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorGreenMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(0, 255, 0);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorBlueMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(0, 0, 255);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorYellowMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(255, 255, 0);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorCyanMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(0, 255, 255);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnColorMagnetaMenu()
{
	// TODO: Add your command handler code here
	currentColor = RGB(255, 0, 255);
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		CClientDC dc(this);
		selectedShape->updateColor(currentColor);
		Invalidate();
	}
}


void CShapeDrawerDlg::OnOptionsSelectshapeMenu()
{
	// TODO: Add your command handler code here
	shape_type = 0;
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
}


void CShapeDrawerDlg::OnEditBringtofrontMenu()
{
	// TODO: Add your command handler code here
	if (selectedShape) {
		int i = selectedShapeP - &shapes[0];
		int size = shapes.size();
		if (i != size-1) {
			utilityFunctions::savePrevState(shapes, lastAction);
			iter_swap(shapes.begin() + i, shapes.begin() + size-1);
			CClientDC dc(this);
			selectedShape->drawMe(&dc);
		}
	}
}


void CShapeDrawerDlg::OnEditDeleteMenu()
{
	// TODO: Add your command handler code here
	if (selectedShape) {
		utilityFunctions::savePrevState(shapes, lastAction);
		int i = selectedShapeP - &shapes[0];
		selectedShape = NULL;
		delete shapes[i];
		shapes.erase(shapes.begin() + i);

		int size = shapes.size();
		for (i = 0;i < size;i++) {
			shapes[i]->unselectShape();
		}
		size = lastAction.size();
		for (i = 0;i < size;i++) {
			lastAction[i]->unselectShape();
		}

		Invalidate();
	}
}


void CShapeDrawerDlg::OnEditUndoMenu()
{
	// TODO: Add your command handler code here
	shapes.swap(lastAction);
	if (selectedShape) {
		int size = shapes.size();
		for (int i = 0; i < size;i++) {
			if (shapes[i]->selectedState()) {
				selectedShapeP = &shapes[i];
				selectedShape = *selectedShapeP;
			}
		}
	}
	Invalidate();
}


void CShapeDrawerDlg::OnOptionsInvalidate()
{
	// TODO: Add your command handler code here
	Invalidate();
}


void CShapeDrawerDlg::OnColorSelectcolorMenu()
{
	// TODO: Add your command handler code here
	cMapI = TRUE;
	CClientDC dc(this);
	cMap.drawMe(&dc);
	cMap.selectedColor(&dc, currentColor);
}


void CShapeDrawerDlg::OnOptionsExitMenu()
{
	// TODO: Add your command handler code here
#ifdef _DEBUG
	newMemState.Checkpoint();
	if (diffMemState.Difference(oldMemState, newMemState))
	{
		TRACE("Memory leaked!\n");
	}
#endif
	exit(0);
}

void CShapeDrawerDlg::OnEditCopyMenu()
{
	// TODO: Add your command handler code here
	if (selectedShape) {
		copy = selectedShape->clone();
	}
}


void CShapeDrawerDlg::OnEditPasteMenu()
{
	// TODO: Add your command handler code here
	if (copy) {
		CPoint offset(15, 15);
		MyShape *temp;
		utilityFunctions::savePrevState(shapes, lastAction);
		copy->offsetShape(offset);
		temp = copy->clone();
		shapes.push_back(temp);
		cMapI = FALSE;
		Invalidate();
	}
}

void CShapeDrawerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (GetKeyState(VK_LCONTROL) & 0x80) {
		if (nChar == _T('z') || nChar == _T('Z')) {
			OnEditUndoMenu();
		}
	}
	if (GetKeyState(VK_LCONTROL) & 0x80) {
		if (nChar == _T('c') || nChar == _T('C')) {
			OnEditCopyMenu();
		}
	}
	if (GetKeyState(VK_LCONTROL) & 0x80) {
		if (nChar == _T('v') || nChar == _T('V')) {
			OnEditPasteMenu();
		}
	}
	if (GetKeyState(VK_DELETE) & 0x80) {
		OnEditDeleteMenu();
	}	


	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}





void CShapeDrawerDlg::OnOptionsSaveMenu()
{
	// TODO: Add your command handler code here
	fstream myFile("data.bin", ios::in | ios::out | ios::trunc | ios::binary);
	int size = shapes.size();
	myFile.write((char*)&size, sizeof(int));
	for (int i = 0;i < size;i++) {
		shapes[i]->writeFile(&myFile);
	}
	myFile.close();
}


void CShapeDrawerDlg::OnOptionsLoadMenu()
{
	// TODO: Add your command handler code here
	fstream myFile("data.bin", ios::in | ios::out | ios::binary);
	if (myFile.is_open()) {
		int *type, *x, *y;
		COLORREF *cColor;
		type = new int;
		x = new int;
		y = new int;
		cColor = new COLORREF;
		MyShape *temp = NULL;

		int *size = new int, *sizeD = new int;

		utilityFunctions::savePrevState(shapes, lastAction);
		int s;
		s = shapes.size();
		for (int i = 0;i < s;i++) {
			delete shapes[i];
		}
		shapes.clear();
		dots.clear();

		myFile.read((char*)size, sizeof(int));
		for (int i = 0;i < *size;i++) {
			myFile.read((char*)type, sizeof(int));
			switch (*type) {
			case 1:
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				startP.x = *x;
				startP.y = *y;
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				endP.x = *x;
				endP.y = *y;
				myFile.read((char*)cColor, sizeof(COLORREF));
				currentColor = *cColor;

				temp = new MyLine(startP, endP, currentColor);

				break;
			case 2:
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				startP.x = *x;
				startP.y = *y;
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				endP.x = *x;
				endP.y = *y;
				myFile.read((char*)cColor, sizeof(COLORREF));
				currentColor = *cColor;

				temp = new MyEllipse(startP, endP, currentColor);
				break;
			case 3:
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				startP.x = *x;
				startP.y = *y;
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				endP.x = *x;
				endP.y = *y;
				myFile.read((char*)cColor, sizeof(COLORREF));
				currentColor = *cColor;

				temp = new MyTriangle(startP, endP, currentColor);
				break;
			case 4:
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				startP.x = *x;
				startP.y = *y;
				myFile.read((char*)x, sizeof(int));
				myFile.read((char*)y, sizeof(int));
				endP.x = *x;
				endP.y = *y;
				myFile.read((char*)cColor, sizeof(COLORREF));
				currentColor = *cColor;

				temp = new MyRectangle(startP, endP, currentColor);
				break;
			case 5:
				myFile.read((char*)sizeD, sizeof(int));
				for (int i = 0;i < *sizeD;i++) {
					myFile.read((char*)x, sizeof(int));
					myFile.read((char*)y, sizeof(int));
					startP.x = *x;
					startP.y = *y;
					dots.push_back(startP);
				}
				myFile.read((char*)cColor, sizeof(COLORREF));
				currentColor = *cColor;

				temp = new MyPolygon(dots, currentColor);
				break;
			default:
				break;
			}
			shapes.push_back(temp);
			Invalidate();
		}
		myFile.close();
		delete x;
		delete y;
		delete type;
		delete cColor;
		delete sizeD;
		delete size;
	}
}


void CShapeDrawerDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);

	// TODO: Add your message handler code here
	if (cMapI) {
		cMapI = FALSE;
		Invalidate();
	}
}
