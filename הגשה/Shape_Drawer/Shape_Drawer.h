
// Shape_Drawer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CShapeDrawerApp:
// See Shape_Drawer.cpp for the implementation of this class
//

class CShapeDrawerApp : public CWinApp
{
public:
	CShapeDrawerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CShapeDrawerApp theApp;
