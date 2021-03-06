
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

#define PI 3.14159265 //constant for pi
#define N_ZERO 0.0000000001 //constant for near zero value

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



CShapeDrawerDlg::CShapeDrawerDlg(CWnd* pParent /*=NULL*/) //constructor
	: CDialog(IDD_SHAPE_DRAWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	//set initial value to flags and members of shape_drawerDLG
	start_up = TRUE;
	is_pressed = FALSE;
	is_poly = FALSE;
	start_mov = FALSE;
	cMapI = FALSE;
	bg_select = FALSE;
	shape_type = 0;		//0 - selection
						//1 - line
						//2 - circle
						//3 - triangle
						//4 - rectangle
						//5 - polygon
	shiftPressed = 0;
	currentColor = RGB(0, 0, 0);
	bgColor = RGB(255, 255, 255);
	selectedShapeP = NULL;

	selectedShape = NULL;
	copy = NULL;

	dots.clear();
	shapes.clear();
	lastAction.clear();
	startP.SetPoint(0, 0);
	endP.SetPoint(0, 0);

}

CShapeDrawerDlg::~CShapeDrawerDlg() { //destructor
	int size;
	size = shapes.size();
	for (int i = 0;i < size;i++) { //release memory the was allocated for each shape
		delete shapes[i];
	}

	size = lastAction.size(); //release memory the was allocated for each copy of shape
	for (int i = 0;i < size;i++) {
		delete lastAction[i];
	}
	
	shapes.clear();
	lastAction.clear();
	//reset vectors

	delete copy; //release memmory that was allocated fro copy/paste option
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
	ON_COMMAND(ID_COLOR_BACKGROUNDCOLOR, &CShapeDrawerDlg::OnColorBackgroundcolorMenu)
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

	//on invalidate
	CClientDC dc(this); //set canvas handler
	CRect rect; 
	GetClientRect(&rect); //rectangle that contains canvas

	CBrush myBrush(bgColor); //set brush for background color
	CBrush *oldBrush; //catch old brush

	oldBrush = dc.SelectObject(&myBrush); //set my brush
	dc.FillRect(rect, &myBrush); //paint background with selected brush
	dc.SelectObject(oldBrush); //return to old brush

	int size = shapes.size();
	for (int i = 0;i < size;i++) { //draw all shapes
		shapes[i]->drawMe(&dc);
	}

	if (cMapI) { //if cMapI flag is true draw color pallet
		cMap.drawMe(&dc);
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

	if (is_pressed) { //if LMB pressed flag is true
		CClientDC dc(this); //set canvas handler
		COLORREF oldPen;
		dc.SelectObject(GetStockObject(DC_PEN));
		oldPen = dc.SetDCPenColor(currentColor); //set pen for current chosen color

		if (cMapI && cMap.isInside(point)) { //if mouse is inside color pallet
			currentColor = GetPixel(dc, point.x, point.y); //get current color
			cMap.selectedColor(&dc, currentColor); //update current color box
		}
		else {
			CPoint points[3]; //0 - top, 1 - left, 2 - right |-> for triangle;
			shiftPressed = GetKeyState(VK_LSHIFT)&0x80; //set LSHIFT pressed flag
			if (shiftPressed) { //if LSHIFT pressed maintain aspect ratio 1:1
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
			case 0: //select shape
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
			case 1: //line
				dc.SetROP2(R2_NOTXORPEN); //avoid overdrawing
				dc.MoveTo(startP);
				dc.LineTo(endP);
				endP = point;
				dc.MoveTo(startP);
				dc.LineTo(endP);
				break; //draw line
			case 2: //circle
				dc.SetROP2(R2_NOTXORPEN); //avoid overdrawing
				dc.MoveTo(startP);
				dc.Ellipse(startP.x, startP.y, endP.x, endP.y);
				endP = point;
				dc.MoveTo(startP);
				dc.Ellipse(startP.x, startP.y, endP.x, endP.y);
				break; //draw circle
			case 3: //triangle
				dc.SetROP2(R2_NOTXORPEN); //avoid overdrawing
				dc.MoveTo(startP);
				utilityFunctions::triangle_Vdots(startP, endP, points[0], points[1], points[2]);
				dc.Polygon(points, 3);
				endP = point;
				dc.MoveTo(startP);
				utilityFunctions::triangle_Vdots(startP, endP, points[0], points[1], points[2]);
				dc.Polygon(points, 3);
				break; //draw triangle
			case 4: //rectangle
				dc.SetROP2(R2_NOTXORPEN); //avoid overdrawing
				dc.MoveTo(startP);
				dc.Rectangle(startP.x, startP.y, endP.x, endP.y);
				endP = point;
				dc.MoveTo(startP);
				dc.Rectangle(startP.x, startP.y, endP.x, endP.y);
				break; //draw rectangle
			case 5: //polygon
				dc.SetROP2(R2_NOTXORPEN); //avoid overdrawing
				dc.MoveTo(startP);
				dc.LineTo(endP);
				endP = point;
				dc.MoveTo(startP);
				dc.LineTo(endP);
				break; //draw lines to set polygon
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
		is_pressed = TRUE; //set flag
		CClientDC dc(this); //set canvas handler
		if (cMapI) { //if color pallet displayed
			if (!cMap.isInside(point)) { //press outside
				CRect pallet;
				cMap.pallet_invldt(pallet);
				cMapI = FALSE;
				bg_select = FALSE;
				InvalidateRect(pallet); //remove pallet
			}
			else if (bg_select && cMap.isInside(point)) { //if bg flag
				bgColor = currentColor; //set bg color
			}
			else if (selectedShape && cMap.isInside(point) && shapes.size()!=0) {
				utilityFunctions::savePrevState(shapes, lastAction);
				selectedShape->updateColor(currentColor); //set shape color
			}
		}
		else {
			if (selectedShape) { //unselect shape
				selectedShape->unselectShape();
			}
			selectedShapeP = utilityFunctions::shapeSrch(point, shapes, &dc); //serach if mouse is inside shape
			if (selectedShapeP == NULL) {
				selectedShape = NULL;
			}
			else {
				selectedShape = *selectedShapeP; //select shape
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
				CRect pallet;
				cMap.pallet_invldt(pallet);
				InvalidateRect(pallet); //remove pallet
			}	
		}
		if (shape_type == 5) { //start draw lines of polygon
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
			currentColor = GetPixel(dc, point.x, point.y);
			cMap.selectedColor(&dc, currentColor);
			if (bg_select) {
				bgColor = currentColor;
			}
			else if (selectedShape && shapes.size()) {
				utilityFunctions::savePrevState(shapes, lastAction);
				selectedShape->updateColor(currentColor);
			}
		}
		else {
			bg_select = FALSE;
			switch (shape_type) { //save current shape
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

	if (is_poly) { //close the polygon
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
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
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
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
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
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
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
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
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
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
	}
	selectedShape = NULL;

	is_poly = FALSE;
}


void CShapeDrawerDlg::OnResetPushMenu()
{
	// TODO: Add your command handler code here
	Invalidate();
}


void CShapeDrawerDlg::OnOptionsSelectshapeMenu()
{
	// TODO: Add your command handler code here
	shape_type = 0;
	if (cMapI) {
		cMapI = FALSE;
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
	}
}


void CShapeDrawerDlg::OnEditBringtofrontMenu()
{
	// TODO: Add your command handler code here
	if (selectedShape) {
		int i = selectedShapeP - &shapes[0]; //find shape index
		int size = shapes.size();
		if (i != size-1) {
			utilityFunctions::savePrevState(shapes, lastAction);
			iter_swap(shapes.begin() + i, shapes.begin() + size-1); //bring shape to front
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
		int i = selectedShapeP - &shapes[0]; //find shape index
		selectedShape = NULL;
		delete shapes[i]; //release memmory alocated for current shape
		shapes.erase(shapes.begin() + i); //delete pointer from vector

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
	shapes.swap(lastAction); //swap vectors for previous action
	if (selectedShape) { //keep seleced shape selelected
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
	cMap.selectedColor(&dc, currentColor); //draw color pallet
}


void CShapeDrawerDlg::OnOptionsExitMenu()
{
	// TODO: Add your command handler code here
	exit(0);
}

void CShapeDrawerDlg::OnEditCopyMenu()
{
	// TODO: Add your command handler code here
	if (selectedShape) {
		copy = selectedShape->clone(); //copy shape
	}
}


void CShapeDrawerDlg::OnEditPasteMenu()
{
	// TODO: Add your command handler code here
	if (copy) {
		CPoint offset(15, 15);
		MyShape *temp;
		utilityFunctions::savePrevState(shapes, lastAction);
		copy->offsetShape(offset); //offset copy right and down
		temp = copy->clone();
		shapes.push_back(temp); //insert it into vector as new shape
		cMapI = FALSE;
		Invalidate();
	}
}

void CShapeDrawerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	//keyboard events
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
	fstream myFile("data.bin", ios::in | ios::out | ios::trunc | ios::binary); //open binary file
	int size = shapes.size();
	myFile.write((char*)&size, sizeof(int)); //write number of shapes
	for (int i = 0;i < size;i++) { //write data of each shape
		shapes[i]->writeFile(&myFile);
	}
	myFile.close(); //close file
}


void CShapeDrawerDlg::OnOptionsLoadMenu()
{
	// TODO: Add your command handler code here
	fstream myFile("data.bin", ios::in | ios::out | ios::binary); //open binary file
	if (myFile.is_open()) { //is sucsessfull
		int *type, *x, *y; //type - shape type, x - of vertex, y - of vertex
		COLORREF *cColor; //cColor - shape color
		type = new int;
		x = new int;
		y = new int;
		cColor = new COLORREF;
		MyShape *temp = NULL; //temp - pointer to loaded shape to be entered into vector

		int *size = new int, *sizeD = new int; //size - number of shapes, sizeD - number of vertexes of polygon

		utilityFunctions::savePrevState(shapes, lastAction);
		int s;
		s = shapes.size(); //cler current vetors
		for (int i = 0;i < s;i++) {
			delete shapes[i];
		}
		shapes.clear();
		dots.clear();

		myFile.read((char*)size, sizeof(int)); //get number of shapes
		for (int i = 0;i < *size;i++) {
			myFile.read((char*)type, sizeof(int)); //get shape type
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
				//load line
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
				//load ellipse
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
				//load triangle
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
				//load rectangle
				break;
			case 5:
				myFile.read((char*)sizeD, sizeof(int)); //get number of vertexes of polygon
				for (int i = 0;i < *sizeD;i++) { //load all vertexe
					myFile.read((char*)x, sizeof(int));
					myFile.read((char*)y, sizeof(int));
					startP.x = *x;
					startP.y = *y;
					dots.push_back(startP);
				}
				myFile.read((char*)cColor, sizeof(COLORREF));
				currentColor = *cColor;

				temp = new MyPolygon(dots, currentColor);
				//load polygon
				break;
			default:
				break;
			}
			shapes.push_back(temp);
		}
		Invalidate();
		myFile.close();
		delete x;
		delete y;
		delete type;
		delete cColor;
		delete sizeD;
		delete size;
		//free memory that was allocated for temp veriables of shapes that was loaded
	}
}


void CShapeDrawerDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);

	// TODO: Add your message handler code here
	if (cMapI) {
		cMapI = FALSE;
		CRect pallet;
		cMap.pallet_invldt(pallet);
		InvalidateRect(pallet); //remove pallet
	}
}


void CShapeDrawerDlg::OnColorBackgroundcolorMenu()
{
	// TODO: Add your command handler code here
	bg_select = TRUE;
	OnColorSelectcolorMenu(); //open color pallet
}

/////////////////////////////////////////////////////////////////////////////////////////////


//initial color settings - not in use

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
