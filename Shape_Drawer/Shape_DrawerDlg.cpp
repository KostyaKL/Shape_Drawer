
// Shape_DrawerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Shape_Drawer.h"
#include "Shape_DrawerDlg.h"
#include "afxdialogex.h"
#include "shapes.h"

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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	start_up = FALSE;
	is_pressed = FALSE;
	is_poly = FALSE;
	shape_type = 0;		//0 - nil
						//1 - line
						//2 - circle
						//3 - triangle
						//4 - rectangle
						//5 - polygon

	items = 0;
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

	if (start_up) {
		CClientDC dc(this);
		CBrush myBrush(RGB(230, 230, 230));
		CBrush *oldBrush;

		CPoint top_left(0, 0);
		CRect rect;
		GetClientRect(&rect);
		CPoint bottom_right = rect.BottomRight();
		bottom_right.y -= 50;
		rect.SetRect(top_left, bottom_right);
		AdjustWindowRect(&rect, WS_OVERLAPPED,1);

		
		oldBrush = dc.SelectObject(&myBrush);
		dc.FillRect(rect, oldBrush);
		dc.SelectObject(oldBrush);

		start_up = FALSE;
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
		CBrush myBrush(RGB(230, 0, 230));
		CBrush *oldBrush;
		oldBrush = dc.SelectObject(&myBrush);
		dc.SelectObject(oldBrush);

		CPoint top, left, right;
		CPoint dots[50];

		switch (shape_type) {
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

			triangle_Vdots(startP, endP, top, left, right);

			dc.MoveTo(top);
			dc.LineTo(left);
			dc.LineTo(right);
			dc.LineTo(top);

			endP = point;
			dc.MoveTo(startP);

			triangle_Vdots(startP, endP, top, left, right);

			dc.MoveTo(top);
			dc.LineTo(left);
			dc.LineTo(right);
			dc.LineTo(top);

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
	}

	CDialog::OnMouseMove(nFlags, point);
}




void CShapeDrawerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	if (shape_type != 5) {
		is_pressed = TRUE;
		startP = point;
		endP = point;
	}
	else {
		if (is_poly == FALSE) {
			is_pressed = TRUE;
			startP = point;
			endP = point;
			is_poly = TRUE;
		}
		else {
			is_pressed = TRUE;

			CClientDC dc(this);

			dc.SetROP2(R2_NOTXORPEN);
			dc.MoveTo(startP);
			dc.LineTo(endP);
			endP = point;
			dc.MoveTo(startP);
			dc.LineTo(endP);

		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CShapeDrawerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (is_pressed) {
		is_pressed = NULL;
		switch (shape_type) {
		case 1:
			//shapes[items] = new MyLine(startP.x, startP.y, endP.x, endP.y);
			items++;
			break;
		case 2:
			//shapes[items] = new MyEllipse(startP.x, startP.y, endP.x, endP.y);
			items++;
			break;
		case 3:
			//shapes[items] = new MyTriangle(startP.x, startP.y, endP.x, endP.y);
			items++;
			break;
		case 4:
			//shapes[items] = new MyRectangle(startP.x, startP.y, endP.x, endP.y);
			items++;
			break;
		case 5:
			startP = endP;
			break;
		default:
			break;
		}
		
		endP = point;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CShapeDrawerDlg::OnLinePresseMenu()
{
	// TODO: Add your command handler code here

	shape_type = 1;
}

void CShapeDrawerDlg::OnCirclePressMenu()
{
	// TODO: Add your command handler code here
	shape_type = 2;
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	menu.CheckMenuItem(ID_NEWSHAPE_CIRCLE, MF_CHECKED);
}


void CShapeDrawerDlg::OnNewshapeTriangle()
{
	// TODO: Add your command handler code here
	shape_type = 3;

}



void CShapeDrawerDlg::OnRectanglePressMenu()
{
	// TODO: Add your command handler code here
	shape_type = 4;
}


void CShapeDrawerDlg::OnPolygonPressMenu()
{
	// TODO: Add your command handler code here
	shape_type = 5;
	is_poly = FALSE;

	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	menu.CheckMenuItem(ID_NEWSHAPE_POLYGON, TRUE);

}



